/**
 * @file Spatial.h
 * @brief Definition of the Spatial class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include <string>
#include "AxisAlignedBoundingBox.h"
#include "GraphicsLayer.h"

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
	 * @param renderer 
	 *			The renderer to use in case this node is visible.
	 */
	void onDraw(GraphicsLayer& renderer) const;
	
	/**
	 * @brief Pure virtual. Called if the node should be drawn.
	 *
	 * @param renderer
	 *			The renderer to use for drawing
	 */
	virtual void draw(GraphicsLayer& renderer) const = 0;

	void setVisible(bool visible);

	const AxisAlignedBoundingBox& getWorldBoundingVolume() const;
protected:
	string name_;
	bool visible_;
	AxisAlignedBoundingBox worldBoundingVolume_;
};

inline void Spatial::setVisible(bool visible){
	visible_ = visible;
}

inline const AxisAlignedBoundingBox& Spatial::getWorldBoundingVolume() const{
	return worldBoundingVolume_;
}
