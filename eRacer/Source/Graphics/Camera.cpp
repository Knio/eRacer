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
