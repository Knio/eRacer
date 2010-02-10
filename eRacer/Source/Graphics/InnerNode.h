/**
 * @file InnerNode.h
 * @brief Definition of the InnerNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include <string>
#include "GraphicsLayer.h"
#include "Spatial.h"

using namespace std;

namespace Graphics {

/**
 * @brief A node in a scene graph that does not contain any geometry.
 *
 * This class is used to spatially partition the scene.
 */
class InnerNode : public Spatial
{
public:
	/**
	 * @brief Constructor stub.
	 *
	 * @param name
	 *			a name for this node to facilitate debugging
	 */
	InnerNode(const string& name);
	virtual ~InnerNode();

	/**
	 * @brief tell all children to check for culling
	 *
	 * @param camera
	 *			The camera to cull against
	 * @param visibleNodes
	 * 			A vector to push all visible nodes to
	 * 
	 * @see Spatial::cullRecursive
	 */
	virtual void cullRecursive(const Camera& camera, vector<const MeshNode*>& visibleNodes) const;

	/**
	 * @brief add a child to this node
	 *
	 * This method also merges the new childs bounding value into
	 * the bounding volume of the node.
	 *
	 * @param spatial
	 *			a base pointer to the new child - must not be 0
	 */
	void addChild(Spatial* spatial);

	/**
	 * @brief remove a child node
	 * 
	 * This method requires a complete recomputation of the bounding
	 * and has thus linear time complexity in the number of children.
	 * The child removed will be destroyed.
	 *
	 * @param spatial
	 *			a base pointer to the child to be removed - must not be 0
	 */
	void removeChild(Spatial* spatial);
private:
	vector<Spatial*> children_;
};

};
