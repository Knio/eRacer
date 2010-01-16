/**
 * @file Camera.cpp
 * @brief
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#include "Camera.h"

Camera::Camera(const Point3& position, const Point3& lookAt, const Vector3& approxUp)
	: position_(position),
	  lookAt_(lookAt)
{
	setUp(approxUp);
}

Camera::~Camera() {
}

void Camera::updatePlanes(){
	//TODO

	//left



	//right

	//bottom

	//top

	//near
	planes_[4].normal = lookAt_-position_;
	D3DXVec3Normalize(planes_[4].normal,planes_[4].normal);
	planes_[4].distance = D3DXVec3Dot(planes_[4].normal, position_+near_*planes_[4].normal);

	//far
	planes_[5].normal = -planes_[4].normal;
	planes_[5].distance = D3DXVec3Dot(planes_[5].normal, position_+far_*planes_[4].normal);
}

