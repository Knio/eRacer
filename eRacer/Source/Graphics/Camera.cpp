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
	SetUp(approxUp);
}

Camera::~Camera() {
}

void Camera::UpdatePlanes(){
	Matrix combinedMatrix = viewMatrix_*projectionMatrix_;

	// Left clipping plane
	planes_[0].normal.x = combinedMatrix._14 + combinedMatrix._11;
	planes_[0].normal.y = combinedMatrix._24 + combinedMatrix._21;
	planes_[0].normal.z = combinedMatrix._34 + combinedMatrix._31;
	planes_[0].distance = combinedMatrix._44 + combinedMatrix._41;

	// Right clipping plane
	planes_[1].normal.x = combinedMatrix._14 - combinedMatrix._11;
	planes_[1].normal.y = combinedMatrix._24 - combinedMatrix._21;
	planes_[1].normal.z = combinedMatrix._34 - combinedMatrix._31;
	planes_[1].distance = combinedMatrix._44 - combinedMatrix._41;

	// Bottom clipping plane
	planes_[2].normal.x = combinedMatrix._14 + combinedMatrix._12;
	planes_[2].normal.y = combinedMatrix._24 + combinedMatrix._22;
	planes_[2].normal.z = combinedMatrix._34 + combinedMatrix._32;
	planes_[2].distance = combinedMatrix._44 + combinedMatrix._42;

	// Top clipping plane
	planes_[3].normal.x = combinedMatrix._14 - combinedMatrix._12;
	planes_[3].normal.y = combinedMatrix._24 - combinedMatrix._22;
	planes_[3].normal.z = combinedMatrix._34 - combinedMatrix._32;
	planes_[3].distance = combinedMatrix._44 - combinedMatrix._42;


	// Near clipping plane
	planes_[4].normal.x = combinedMatrix._13;
	planes_[4].normal.y = combinedMatrix._23;
	planes_[4].normal.z = combinedMatrix._33;
	planes_[4].distance = combinedMatrix._43;

	// Far clipping plane
	planes_[5].normal.x = combinedMatrix._14 - combinedMatrix._13;
	planes_[5].normal.y = combinedMatrix._24 - combinedMatrix._23;
	planes_[5].normal.z = combinedMatrix._34 - combinedMatrix._33;
	planes_[5].distance = combinedMatrix._44 - combinedMatrix._43;
}

const Plane& Camera::GetPlane(int planeIndex) const {
	assert(0 <= planeIndex);
	assert(planeIndex < 6);
	return planes_[planeIndex];
}

void Camera::SetViewMatrix(const Matrix& viewMatrix){
	//float ignore;
	//Matrix rotation;

	//Decompose(viewMatrix, position_, rotation, ignore, ignore, ignore);

	viewMatrix_ = viewMatrix;
	
	position_ = ORIGIN;
	lookAt_ = Z;
	up_ = Y;

	transformAffine(viewMatrix,position_);
	transformAffine(viewMatrix,lookAt_);
	transformAffine(viewMatrix,up_);
	approxUp_ = up_;

}

}
