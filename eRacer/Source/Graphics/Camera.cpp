/**
 * @file Camera.cpp
 * @brief
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#include "Camera.h"

namespace Graphics {

Camera::Camera(const Point3& position, const Point3& lookAt, const Vector3& approxUp)
	: position_(position),
	  lookAt_(lookAt),
	  near_(1.0f),
	  far_(100.0f),
	  aspectRatio_(0.75f),
	  fovY_(PI*0.25f)
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
	normalize(planes_[4].normal);
	planes_[4].distance = dot(planes_[4].normal, position_+near_*planes_[4].normal);

	//far
	planes_[5].normal = -planes_[4].normal;
	planes_[5].distance = dot(planes_[5].normal, position_+far_*planes_[4].normal);
}

};
