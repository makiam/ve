/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Geometry/GeometryBuffer.h"
#include "Core/Log.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

VertexElement::VertexElement(VertexAttribute::Enum attribute,
	VertexDataType::Enum type, uint8 components)
	: stride(-1)
	, offset(0)
	, size(0)
{
	this->attribute = attribute;
	this->type = type;
	this->components = components;
}

//-----------------------------------//

static int GetVertexDataTypeSize(VertexDataType::Enum type)
{
	switch(type)
	{
	case VertexDataType::Float:   return sizeof(float);
	case VertexDataType::Integer: return sizeof(int);
	case VertexDataType::Byte:    return sizeof(uint8);
	}

	LogAssert("Unknown vertex type");

	return 0;
}

//-----------------------------------//

uint8 VertexElement::getSize() const
{
	return components * GetVertexDataTypeSize(type);
}

//-----------------------------------//

void VertexDeclaration::add(const VertexElement& elem)
{
	decls.push_back(elem);
}

//-----------------------------------//

void VertexDeclaration::add(const VertexElementP& pod)
{
	VertexElement elem(pod.attribute, pod.type, pod.components);
	decls.push_back(elem);
}

//-----------------------------------//

void VertexDeclaration::add(VertexAttribute::Enum attribute, int numComponents)
{
	VertexElement elem(attribute, VertexDataType::Float, numComponents);
	decls.push_back(elem);
}

//-----------------------------------//

VertexElement* VertexDeclaration::find(VertexAttribute::Enum attribute) const
{
	for(size_t i = 0; i < decls.size(); i++)
	{
		const VertexElement& elem = decls[i];
		if(elem.attribute != attribute) continue;
		
		return (VertexElement*) &elem;
	}
	
	return nullptr;
}

//-----------------------------------//

void VertexDeclaration::reset()
{
	decls.clear();
}

//-----------------------------------//

uint8 VertexDeclaration::getVertexSize() const
{
	uint8 totalSize = 0;

	for(size_t i = 0; i < decls.size(); i++)
	{
		const VertexElement& elem = decls[i];
		totalSize += elem.getSize();
	}

	return totalSize;
}

//-----------------------------------//

uint8 VertexDeclaration::getOffset(VertexAttribute::Enum attribute) const
{
	uint8 totalOffset = 0;

	for(size_t i = 0; i < decls.size(); i++)
	{
		const VertexElement& elem = decls[i];

		if(elem.attribute == attribute) return totalOffset;

		// If there is no stride, the elements are tightly packed.
		if(elem.stride == 0) continue;

		totalOffset += elem.getSize();
	}

	return totalOffset;
}

//-----------------------------------//

void VertexDeclaration::calculateStrides()
{
	uint8 vertexSize = getVertexSize();

	for(size_t i = 0; i < decls.size(); ++i)
	{
		VertexElement& elem = decls[i];
		elem.stride = vertexSize;
		elem.offset = getOffset(elem.attribute);
	}
}

//-----------------------------------//

GeometryBuffer::GeometryBuffer()
	: usage(BufferUsage::Static)
	, access(BufferAccess::Read)
	, indexSize(16)
	, needsRebuild(false)
	, hash(0)
{
}

//-----------------------------------//

GeometryBuffer::GeometryBuffer(BufferUsage::Enum usage, BufferAccess::Enum access)
{
}

//-----------------------------------//

GeometryBuffer::~GeometryBuffer()
{
}

//-----------------------------------//

void GeometryBuffer::clear()
{
	data.clear();
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::set(VertexAttribute::Enum attribute, uint8* buf, uint32 size)
{
	VertexElement* elem = declarations.find(attribute);
	if( !elem ) return;

	if(elem->size != size)
	{
		LogAssert("Vertex element sizes do not match");
		return;
	}

	std::copy(buf, buf+size, data.begin() + elem->offset);
}

//-----------------------------------//

void GeometryBuffer::set(uint8* buf, uint32 size)
{
	clear();
	add(buf, size);
}

//-----------------------------------//

void GeometryBuffer::add(uint8* buf, uint32 size)
{
	data.resize(data.size() + size);
	std::copy(buf, buf+size, data.end() - size);
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::setIndex(uint8* buf, uint32 size)
{
	indexData.clear();
	addIndex(buf, size);
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::addIndex(uint8* buf, uint32 size)
{
	indexData.resize(indexData.size() + size);
	std::copy(buf, buf+size, indexData.end() - size);
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::addIndex(uint16 index)
{
	return addIndex((uint8*) &index, sizeof(uint16));
}

//-----------------------------------//

bool GeometryBuffer::isIndexed() const
{
	return !indexData.empty();
}

//-----------------------------------//

uint32 GeometryBuffer::getSizeVertices() const
{
	if( declarations.decls.empty() ) return 0;

	uint8 sizeVertex = declarations.getVertexSize();
	
	// Assume all separate data is the same size.
	if( sizeVertex == 0 )
	{
		const VertexElement& elem = declarations.decls[0];
		return elem.size / elem.getSize();
	}

	return data.size() / sizeVertex;
}

//-----------------------------------//

uint32 GeometryBuffer::getSizeIndices() const
{
	uint8 indexSizeBytes = indexSize / 8;
	assert( indexSizeBytes != 0 );

	uint32 numIndices = indexData.size() / indexSizeBytes;
	
	return numIndices;
}

//-----------------------------------//

float* GeometryBuffer::getAttribute(VertexAttribute::Enum attribute, uint32 i) const
{
	VertexElement* elem = declarations.find(attribute);
	if( !elem ) return nullptr;

	if(elem->stride == 0)
	{
		uint8 elemSize = elem->getSize();

		const uint8* loc = &data.front() + elem->offset + (elemSize * i);
		return (float*) loc;
	}

	// Calculate the starting position of the attribute data.
	uint8 totalSize = declarations.getVertexSize();
	uint8 localOffset = declarations.getOffset(elem->attribute);

	// TODO: Check the bounds and return null in that case too.

	const uint8* loc = &data.front() + (totalSize * i) + localOffset;
	return (float*) loc;
}

//-----------------------------------//

int8 GeometryBuffer::getAttributeStride(VertexAttribute::Enum attribute) const
{
	VertexElement* elem = declarations.find(attribute);
	if( !elem ) return -1;

	return elem->stride;
}

//-----------------------------------//

void GeometryBuffer::forceRebuild()
{
	needsRebuild = true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END