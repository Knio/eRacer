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
#include "AxisAlignedBoundingBox.h"
#include "Camera.h"

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

	bool visible;

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
	 * @param visibleNodes
	 * 			A vector to push all visible nodes to
	 */
	void cull(const Camera& camera, vector<const MeshNode*>& visibleNodes) const;
	const AxisAlignedBoundingBox& getWorldBoundingVolume() const;

protected:
	/**
	 * @brief Pure virtual. Called if the node should not be culled.
	 *
	 * @param camera
	 *			The camera to cull against
	 * @param visibleNodes
	 * 			A vector to push all visible nodes to
	 */
	virtual void cullRecursive(const Camera& camera, vector<const MeshNode*>& visibleNodes) const = 0;
	string name_;	
	AxisAlignedBoundingBox worldBoundingVolume_;
};

inline const AxisAlignedBoundingBox& Spatial::getWorldBoundingVolume() const{
	return worldBoundingVolume_;
}

};
