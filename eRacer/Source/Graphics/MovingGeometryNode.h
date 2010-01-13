/**
 * @file MovingGeometryNode.h
 * @brief Definition of the MovingGeometryNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "GeometryNode.h"

/**
 * @brief a node containing moving geometry
 */
class MovingGeometryNode : public GeometryNode
{
public:
	MovingGeometryNode(const string& name);
	virtual ~MovingGeometryNode();

	void setTransform(const Matrix& transform);
private:
	AxisAlignedBoundingBox localBoundingVolume_;
};
