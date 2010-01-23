/**
 * @file MovingGeometry.h
 * @brief Definition of the MovingGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Geometry.h"

namespace Graphics {


/**
 * @brief a node containing moving geometry
 *
 * Moving geometry is geometry that may change its transform over time. 
 * This requires updating of bounding boxes.
 * Moving geometry may not change its mesh over time! Another subclass should be dedicated 
 * to this if necessary.
 */
class MovingGeometry : public Geometry
{
public:
	MovingGeometry(const string& name);
	virtual ~MovingGeometry();

	void setTransform(const Matrix& transform);

	/**
	 * @brief setter for the mesh
	 *
	 * This setter can only be called once. It also updates the bounding volumes.
	 *
	 * @param mesh
	 *			a pointer to the mesh for this geometry
	 */
	virtual void SetMesh(const LPD3DXMESH mesh);
private:
	AxisAlignedBoundingBox modelBoundingVolume_;
};


};
