/**
 * @file MovingGeometry.h
 * @brief Definition of the MovingGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "StaticGeometry.h"

namespace Graphics {


/**
 * @brief a node containing moving geometry
 */
class MovingGeometry : public StaticGeometry
{
public:
	MovingGeometry(const string& name);
	virtual ~MovingGeometry();

	void setTransform(const Matrix& transform);
private:
	AxisAlignedBoundingBox localBoundingVolume_;
};


};
