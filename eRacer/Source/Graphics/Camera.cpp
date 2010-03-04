/**
 * @file Camera.cpp
 * @brief
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#include "Camera.h"

namespace Graphics {

Camera::Camera(const Point3& position, const Point3& lookAt, const Vector3& approxUp, bool perspective)
	: position_(position),
	  lookAt_(lookAt),
	  near_(1.0f),
	  far_(100.0f),
	  aspectRatio_(0.75f),
	  fovY_(PI*0.25f),
	  height_(800),
	  perspective_(perspective)
{
	SetUp(approxUp);
}

Camera::~Camera() {
}

void Camera::SetFrame(const Point3& position, const Point3& lookAt, const Vector3& approxUp, bool perspective){
	position_ = position;
	lookAt_ = lookAt;
  perspective_ = perspective;
	SetUp(approxUp);
	UpdateView();
	UpdateProjection();
	UpdatePlanes();
}


void Camera::SetPosition(const Point3& position){
	position_ = position;

	//adjust up so that it is perpendicular to view direction
	UpdateUp();

	UpdateView();
	UpdatePlanes();
}

void Camera::SetLookAt(const Point3& lookAt){
	lookAt_ = lookAt;

	//adjust up so that it is perpendicular to view direction
	UpdateUp();

	UpdateView();
	UpdatePlanes();
}


void Camera::SetUp(const Vector3& approxUp)
{ 
	approxUp_ = approxUp; 
	UpdateUp(); 
	
	UpdateView();
	UpdatePlanes();
}

void Camera::UpdateUp(){
	up_ = approxUp_;
	Vector3 viewDirection = lookAt_ - position_;
	normalize(viewDirection);
	normalize(up_);
	up_ = cross(cross(viewDirection, up_),viewDirection);
}

void Camera::SetAspectRatio(float aspectRatio){
	assert(aspectRatio>0);
	aspectRatio_ = aspectRatio;
	UpdateProjection();
	UpdatePlanes();
}

void Camera::SetFovY(float fovY){
	assert(fovY>0);
	assert(fovY<PI);
	assert(perspective_);
	fovY_ = fovY;
	UpdateProjection();
	UpdatePlanes();
}

void Camera::SetHeight(float height){
	assert(height>0);
	assert(!perspective_);
	height_ = height;
	UpdateProjection();
	UpdatePlanes();
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
	viewMatrix_ = viewMatrix;
	
	position_ = ORIGIN;
	lookAt_ = Z;
	up_ = Y;

	mul1(viewMatrix, position_);
	mul1(viewMatrix, lookAt_);
	mul0(viewMatrix, up_);
	approxUp_ = up_;

}

void Camera::UpdateProjection(){
	if(perspective_)
	   D3DXMatrixPerspectiveFovLH(&projectionMatrix_,
	                              fovY_,
	                              aspectRatio_,
	                              near_,
	                              far_ );
	else
	   D3DXMatrixOrthoLH(&projectionMatrix_,
	                              height_*aspectRatio_,
		                            height_,
	                              near_,
	                              far_ );
}


}
