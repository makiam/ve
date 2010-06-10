/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Node.h"

namespace vapor {

//-----------------------------------//

/**
 * This event gets sent out whenever an operation that has the corresponding
 * delegate declared in the Group class is executed. Examples of such opera-
 * tions are adding / removing nodes from the group. This can be useful when
 * monitoring of scene changes is needed (for example editor applications).
 */

struct GroupEvent
{
	NodePtr node;
};

//-----------------------------------//

/**
 * Groups are nodes that have the special property of being able to be
 * attached to each other. If you attach a node to another node, they will
 * be in a hierarchical relationship and the parent node will influence the 
 * child node transformations.
 */

class VAPOR_API Group : public Node
{
public:

	Group( const std::string& name );

	// Adds child to the end of the child list.  Returns the index of
	// the new child node that has been added.
	virtual int add( const NodePtr& child );

	// Inserts child after child at i'th index.  Returns the index of
	// the new child inserted, if there is no existing
	// child at 'i' then -1 is returned.
	virtual int insert( uint i, const NodePtr& child );

	// Removes node from the group.
	virtual bool remove( const NodePtr& );

	// Removes child at index i, if i is out of bounds returns false,
	// if i is in bounds and child was removed, returns true
	virtual bool remove( uint i );

	// Returns null pointer if cannot find index i
	NodePtr get( uint i ) const;

	// Gets the children of this group.
	IMPLEMENT_GETTER(Children, const std::vector<NodePtr>&, children)

	// Returns index for child with the same pointer, if child cannot
	// be found, -1 is returned
	int index( const NodePtr& child ) const;

	// Returns number of children in this group
	int count( ) const;

	// Updates this group of nodes.
	virtual void update( double delta );

	DECLARE_SERIALIZABLE();

	//-----------------------------------//
	// Events
	//-----------------------------------//

	fd::delegate< void( const GroupEvent& ) > onNodeAdded;
	fd::delegate< void( const GroupEvent& ) > onNodeRemoved;
	fd::delegate< void( const GroupEvent& ) > onNodeInserted;

protected:

	// Holds a group of nodes.
	typedef std::vector<NodePtr> NodeVector;
	NodeVector children;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Group );

//-----------------------------------//

} // end namespace