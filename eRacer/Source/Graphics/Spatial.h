/**
 * @file Spatial.h
 * @brief Definition of the Spatial class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include <string>
#include <vector>
#include "BoundingSphere.h"
#include "Camera.h"
#include "Renderable.h"

namespace Graphics {


class MeshNode;

/**
 * @brief Abstract base class for all nodes in a scene graph.
 */
class Spatial
{
public:
	/**
	 * @brief Constructor stub.
	 *
	 * @param name
	 *			a name for this node to facilitate debugging
	 */
	Spatial(const string& name);

	/**
	 * @brief Destructor stub.
	 */
	virtual ~Spatial();

	/**
	 * @brief Visibility and culling.
	 *
	 * @param camera
	 *			The camera to cull against
	 * @param visibleRenderables
	 * 			A vector to push all visible nodes to
	 */
	void cull(const Camera& camera, vector<const Renderable*>& visibleRenderables) const;


	const BoundingSphere& getWorldBounds() const;

	bool visible;
protected:
	/**
	 * @brief Pure virtual. Called if the node should not be culled.
	 *
	 * @param camera
	 *			The camera to cull against
	 * @param visibleNodes
	 * 			A vector to push all visible nodes to
	 */
	virtual void cullRecursive(const Camera& camera, vector<const Renderable*>& visibleRenderables) const = 0;
	string name_;	
	BoundingSphere worldBounds_;
};

inline const BoundingSphere& Spatial::getWorldBounds() const{
	return worldBounds_;
}

};
