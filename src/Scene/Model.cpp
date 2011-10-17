/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Model.h"
#include "Scene/Transform.h"
#include "Scene/Entity.h"
#include "Resources/Mesh.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"
#include "Resources/Bone.h"
#include "Resources/Attachment.h"
#include "Render/Device.h"
#include "Render/RenderContext.h"
#include "Render/MeshManager.h"
#include "Render/Program.h"
#include "Render/ProgramManager.h"
#include "Math/Helpers.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(SkinningMode)
	ENUM(Native)
	ENUM(CPU)
	ENUM(GPU)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Model, Geometry)
	FIELD_PRIMITIVE(6, float, animationSpeed)
	FIELD_PRIMITIVE(7, bool, animationEnabled)
	FIELD_ENUM_SETTER(8, SkinningMode, skinningMode, SkinningMode)
	FIELD_CLASS_PTR_SETTER(9, Mesh, MeshHandle, mesh, Handle, Mesh)
REFLECT_CLASS_END()

//-----------------------------------//

Model::Model()
{
	init();
}

//-----------------------------------//

Model::Model( const MeshHandle& meshHandle )
	: mesh(meshHandle)
{
	init();
}

//-----------------------------------//

Model::~Model()
{
}

//-----------------------------------//

void Model::init()
{
	updateSkin = false;
	modelBuilt = false;
	debugRenderable = nullptr;
	skinningMode = SkinningMode::Native;
	pmesh = nullptr;

	animationEnabled = true;
	animationFadeTime = 0;
	animationCurrentFadeTime = 0;
	animationSpeed = 1;

	animations.clear();
	attachments.clear();
	bones.clear();
	renderables.clear();
}

//-----------------------------------//

void Model::setMesh(const MeshHandle& meshHandle)
{
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		rend->onPreRender.Bind(this, &Model::onRender);
	}

	init();
	mesh = meshHandle;
}

//-----------------------------------//

void Model::setSkinningMode( SkinningMode::Enum mode )
{
	skinningMode = mode;
	updateSkin = true;
}

//-----------------------------------//

void Model::updateSkinning()
{
	if( !pmesh->isAnimated() ) return;
	
	bool isCPU = skinningMode == SkinningMode::CPU;
	String shader = isCPU ? "VertexLit" : "VertexLitSkinned";
	
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
	
		Material* material = rend->getMaterial().Resolve();
		material->setProgram(shader);
	}

	if( !isCPU ) rebuildPositions();
}

//-----------------------------------//

void Model::update( float delta )
{
	pmesh = mesh.Resolve();

	if( !pmesh || !pmesh->isLoaded() )
		return;

	if( !modelBuilt ) build();

	if(updateSkin)
	{
		updateSkinning();
		updateSkin = false;
	}

	if( pmesh->isAnimated() )
	{
		if( animations.empty() )
		{
			setAnimation( pmesh->getBindPose() );
		}

		if( animationEnabled )
		{
			updateAnimations(delta);
			updateFinalAnimationBones();
		}

		updateAttachments();
	}

	//updateDebugRenderable();
	Geometry::update(delta);
}

//-----------------------------------//

void Model::prepareSkinning()
{
	if( !pmesh->isAnimated() ) return;

	size_t numBones = pmesh->getSkeleton()->getBones().size();
	bones.resize( numBones );
}

//-----------------------------------//

void Model::rebuildPositions()
{
	if( renderables.empty() ) return;

	const RenderablePtr& rend = renderables[0];
	const GeometryBufferPtr& gb = rend->getGeometryBuffer();

	const std::vector<Vector3>& meshPositions = pmesh->position;
	gb->set( VertexAttribute::Position, meshPositions );
	
	gb->forceRebuild();
}

//-----------------------------------//

void Model::build()
{
	if( modelBuilt ) return;

	MeshManager* meshes = GetRenderDevice()->getActiveContext()->meshManager;
	
	RenderablesVector renderables;

	if( !meshes->getMeshRenderables(pmesh, renderables) )
		return;

	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& rend = renderables[i];
		const MaterialHandle& handle = rend->getMaterial();

		Material* material = handle.Resolve();
		
		if( material && material->isBlendingEnabled() )
			rend->setRenderLayer(RenderLayer::Transparency);

		rend->onPreRender.Bind(this, &Model::onRender);
		
		addRenderable( rend );
	}

	// Updates the model bounding box.
	updateBounds();
	
	// Re-compute the bounding box.
	TransformPtr transform = getEntity()->getTransform();
	transform->markBoundingVolumeDirty();

	prepareSkinning();

	modelBuilt = true;
}

//-----------------------------------//

void Model::updateAnimations(float delta)
{
	for( size_t i = 0; i < animations.size(); i++ )
	{
		AnimationState& state = animations[i];

		updateAnimationBones(state);
		updateAnimationTime(state, delta);
	}

	if( animationCurrentFadeTime > animationFadeTime )
	{
		animationFadeTime = 0;
		animationCurrentFadeTime = 0;

		if( animations.size() >= 2 )
			animations.erase( animations.begin()+1 );
	}

	if( animationFadeTime > 0 )
		animationCurrentFadeTime += float(delta);
}

//-----------------------------------//

void Model::updateAnimationTime(AnimationState& state, float delta)
{
	const AnimationPtr& animation = state.animation;
	float& animationTime = state.animationTime;
	float totalTime = animation->getTotalTime();

	if( MathFloatCompare(animationTime, totalTime) )
	{
		animationTime = 0;
			
		if( !animation->isLooped() )
			animationEnabled = false;
	}
	else
	{
		animationTime += float(delta * 10 * animationSpeed);

		if( animationTime > totalTime )
		{
			animationTime = animation->getTotalTime();
		}
	}
}

//-----------------------------------//

void Model::updateAnimationBones(AnimationState& state)
{
	const AnimationPtr& animation = state.animation;
	float animationTime = state.animationTime;

	std::vector<Matrix4x3>& bones = state.bonesMatrix;
	const KeyFramesMap& keyFrames = animation->getKeyFrames();

	KeyFramesMap::const_iterator it;
	for( it = keyFrames.begin(); it != keyFrames.end(); it++ )
	{
		const BonePtr& bone = it->first;
		
		const Matrix4x3& matKey = animation->getKeyFrameMatrix(bone, animationTime);	
		Matrix4x3 matBone = matKey * bone->relativeMatrix;

		if( bone->indexParent != -1 )
			bones[bone->index] = matBone * bones[bone->indexParent];
		else
			bones[bone->index] = matBone;
	}
}

//-----------------------------------//

void Model::updateFinalAnimationBones()
{
	if( pmesh->animations.empty() )
	{
		const BonesVector& skeletonBones = pmesh->skeleton->getBones();
		
		for( size_t i = 0; i < bones.size(); i++ )
			bones[i] = skeletonBones[i]->absoluteMatrix;
		
		return;
	}

	for( size_t i = 0; i < bones.size(); i++ )
	{
		if( animations.size() >= 2 )
		{
			bones[i] = Matrix4x3::lerp(
				animations[0].bonesMatrix[i], animations[1].bonesMatrix[i],
				1 - (animationCurrentFadeTime / animationFadeTime));
		}
		else
		{
			bones[i] = animations[0].bonesMatrix[i];
		}
	}
}

//-----------------------------------//

void Model::updateAttachments()
{
	for( size_t i = 0; i < attachments.size(); i++ )
	{
		const AttachmentPtr& attachment = attachments[i];
		const EntityPtr& node = attachment->node;
		const BonePtr& bone = attachment->bone;

		const TransformPtr& transform = node->getTransform();
		transform->setAbsoluteTransform( bones[bone->index] );
	}
}

//-----------------------------------//

void Model::updateBounds()
{
	bounds = pmesh->getBoundingVolume();
}

//-----------------------------------//

void Model::setAnimation(const AnimationPtr& animation)
{
	assert( pmesh != nullptr );

	if( !pmesh->isAnimated() ) return;

	if( !animation ) return;

	AnimationState state;
	state.animation = animation;
	state.animationTime = 0;

	size_t numBones = pmesh->getSkeleton()->getBones().size();
	state.bonesMatrix.resize(numBones);

	if( animations.empty() )
		animations.push_back(state);
	else
		animations[0] = state;

	animationEnabled = true;
}

//-----------------------------------//

void Model::setAnimation(const std::string& name)
{
	if( !pmesh ) return;

	AnimationPtr animation = pmesh->findAnimation(name);
	setAnimation(animation);
}

//-----------------------------------//

void Model::setAnimationFade(const std::string& name, float fadeTime)
{
	assert( pmesh != nullptr );

	if( !pmesh->isAnimated() )
		return;

	if( animations.size() >= 2 )
		animations[1] = animations[0];
	else
		animations.push_back( animations[0] );
		
	setAnimation(name);
	animationFadeTime = fadeTime;
	animationCurrentFadeTime = 0;
}

//-----------------------------------//

bool Model::isHardwareSkinned()
{
	RenderPipeline::Enum pipeline = GetRenderDevice()->getPipeline();
	
	bool isFixed = pipeline == RenderPipeline::Fixed;
	bool isCPU = skinningMode == SkinningMode::CPU;

	return !(isFixed || isCPU);
}

//-----------------------------------//

void Model::onRender(const RenderState&)
{
	if( !pmesh || !pmesh->isLoaded() || !pmesh->isAnimated() )
		return;

	if( !isHardwareSkinned() )
	{
		if( renderables.empty() ) return;

		const RenderablePtr& rend = renderables[0];
		
		const GeometryBufferPtr& gb = rend->getGeometryBuffer();
		doSkinning(gb);	
	}
	else
	{
		setupShaderSkinning();
	}
}

//-----------------------------------//

void Model::doSkinningRaw(Vector3* positions)
{
	const std::vector<Vector3>& meshPositions = pmesh->position;

	for(size_t i = 0; i < meshPositions.size(); i++)
	{
		int32 boneIndex = (int32) pmesh->boneIndices[i];

		Vector3* pos = positions + i;
		*pos = bones[boneIndex] * meshPositions[i];
	}
}

//-----------------------------------//

void Model::doSkinning(const GeometryBufferPtr& gb)
{
	VertexElement* elemPosition = gb->declarations.find(VertexAttribute::Position);
	if( !elemPosition ) return;

	// Can only handle buffers with 3 components and float.
	if(elemPosition->components != 3 || elemPosition->type != VertexType::Float)
		return;

	uint32 numVertices = gb->getSizeVertices();

	if( pmesh->position.size() != numVertices )
	{
		LogDebug("Skinned mesh and its buffer have different vertices");
		return;
	}

	const std::vector<Vector3>& meshPositions = pmesh->position;

	for(size_t i = 0; i < meshPositions.size(); i++)
	{
		int32 boneIndex = (int32) pmesh->boneIndices[i];

		Vector3* pos = (Vector3*) gb->getAttribute(VertexAttribute::Position, i);
		*pos = bones[boneIndex] * meshPositions[i];
	}

	gb->forceRebuild();
}

//-----------------------------------//

void Model::setupShaderSkinning()
{
	// Setup matrices.
	std::vector<Matrix4x4> matrices;
	matrices.reserve( bones.size() );

	for( size_t i = 0; i < bones.size(); i++ )
	{
		const Matrix4x3& bone = bones[i];
		matrices.push_back( Matrix4x4(bone) );
	}

	// Send them to the uniform buffer.
	const std::vector<RenderablePtr>& rends = getRenderables();

	for( size_t i = 0; i < rends.size(); i++ )
	{
		const RenderablePtr& rend = rends[i];
		
		const UniformBufferPtr& ub = rend->getUniformBuffer();
		ub->setUniform("vp_BonesMatrix", matrices);
	}
}

//-----------------------------------//

void Model::setAttachment(const String& boneName, const EntityPtr& node)
{
	if(!pmesh)
		return;

	SkeletonPtr skeleton = pmesh->getSkeleton();
	assert( skeleton != nullptr );

	BonePtr bone = skeleton->findBone(boneName);
	
	if( !bone )
		return;

	AttachmentPtr attachment = new Attachment();
	
	attachment->bone = bone;
	attachment->node = node;

	attachments.push_back(attachment);
}

//-----------------------------------//
#if 0
void Model::updateDebugRenderable() const
{
	if( !debugRenderable )
		return;

	if( !pmesh->isAnimated() )
		return;

	GeometryBuffer gb = debugRenderable->getVertexBuffer();

	std::vector<Vector3> pos;
	std::vector<Vector3> colors;

	const SkeletonPtr& skel = pmesh->getSkeleton();

	uint numBones = skel->bones.size();
	for( size_t i = 0; i < numBones; i++ )
	{
		BonePtr& bone = skel->bones[i];

		Vector3 vertex;
		Vector3 parentVertex;

		if( bone->indexParent == -1 )
			continue;
		
		parentVertex = bones[bone->indexParent]*parentVertex;	

		pos.push_back( bones[bone->index]*vertex );
		colors.push_back( Color::Blue );

		pos.push_back( parentVertex);
		colors.push_back( Color::Blue );
	}

	gb->set( VertexAttribute::Position, pos );
	gb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

RenderablePtr Model::createDebugRenderable() const
{
	assert( !debugRenderable );

	MaterialHandle handleMaterial = MaterialCreate(AllocatorGetHeap(), "SkeletonDebug");

	Material* material = handleMaterial.Resolve();
	material->setDepthTest(false);

	GeometryBuffer gb = Allocate(VertexBuffer, AllocatorGetHeap());
	
	RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
	renderable->setPrimitiveType(PolygonType::Lines);
	renderable->setGeometryBuffer(gb);
	renderable->setMaterial(handleMaterial);

	return renderable;
}
#endif
//-----------------------------------//

NAMESPACE_ENGINE_END