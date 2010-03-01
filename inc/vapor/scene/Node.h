/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Geometry.h"

namespace vapor { namespace scene {

//-----------------------------------//

class Node;

TYPEDEF_SHARED_POINTER_FROM_CLASS( Node );
TYPEDEF_SHARED_WEAK_POINTER_FROM_CLASS( Node );

typedef std::map< std::string, ComponentPtr > ComponentMap;
typedef std::pair< const std::string, ComponentPtr > ComponentMapPair;

//-----------------------------------//

/**
 * Represents a node, that is, an entity of the world held in the scene. 
 * Entities will need to have components to get different properties.
 * For example you can add a Transform component to give the entity a
 * placement in the world. Or you can add a Physics component that will
 * make it react to the world gravity and make it obey the physics laws.
 */

class VAPOR_API Node : public std::enable_shared_from_this< Node >, 
	private boost::noncopyable, public Serializable
{
public:

	explicit Node();
	explicit Node( const std::string& name );
	
	virtual ~Node();

	// Sets the parent of the node.
	void setParent( NodePtr parent );

	// Gets the parent of the node.
	NodePtr getParent() const;

	// Adds a component to this node.
	bool addComponent( const ComponentPtr& component );

	// Gets a component from this node.
	ComponentPtr getComponent( const std::string& type );

	// Returns all the registered components in this node.
	const ComponentMap& getComponents();

	// Gets a component from this node.
	template <typename T>
	std::shared_ptr<T> getComponent( const std::string& type )
	{
		ComponentPtr cmp = getComponent( type );
		return std::static_pointer_cast< T >( cmp );
	}

	// Removes a component from this node.
	bool removeComponent( const std::string& name );

	// Updates all the components of the node.
	virtual void update( double delta );

	// Gets the name of the node.
	virtual const std::string& getName() const;

	// Sets the name of the node.
	virtual void setName( const std::string& name );

	// Gets the associated transform component (if any).
	TransformPtr getTransform();

	// Gets the geometries components in the node.
	const std::vector< GeometryPtr >& getGeometry();

	// Is this node visible?
	bool isVisible() const;

	// Sets the visibility of this node.
	void setVisible( bool visible );

	DECLARE_SERIALIZABLE();

private:

	// The name of the node.
	std::string name;

	// Holds the components of the node.
	ComponentMap components;

	// Points to the parent node (if any). 
	NodeWeakPtr parent;
	
	// Caches the geometries nodes (for faster lookup when rendering).
	std::vector< GeometryPtr > geometries;

	// Visibility
	bool _isVisible;
};

//-----------------------------------//

} } // end namespaces