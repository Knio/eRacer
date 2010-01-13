/**
 * @file MovingGeometryNode.h
 * @brief Definition of the MovingGeometryNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

/**
 * @brief a node containing moving geometry
 */
class MovingGeometryNode
{
public:
	MovingGeometryNode(const string& name);
	virtual ~MovingGeometryNode();

	void setTransform(const SRTTransform& transform);
private:
	AxisAlignedBoundingVolume localBoundingVolume_;
};
