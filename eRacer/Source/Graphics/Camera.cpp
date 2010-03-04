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
	Matrix combinedMatrix = projectionMatrix_*viewMatrix_;
   
	// Left clipping plane
	planes_[PI_LEFT].normal.x = combinedMatrix._14 + combinedMatrix._11;
	planes_[PI_LEFT].normal.y = combinedMatrix._24 + combinedMatrix._21;
	planes_[PI_LEFT].normal.z = combinedMatrix._34 + combinedMatrix._31;
	planes_[PI_LEFT].distance = combinedMatrix._44 + combinedMatrix._41;

	// Right clipping plane
	planes_[PI_RIGHT].normal.x = combinedMatrix._14 - combinedMatrix._11;
	planes_[PI_RIGHT].normal.y = combinedMatrix._24 - combinedMatrix._21;
	planes_[PI_RIGHT].normal.z = combinedMatrix._34 - combinedMatrix._31;
	planes_[PI_RIGHT].distance = combinedMatrix._44 - combinedMatrix._41;

	// Bottom clipping plane
	planes_[PI_BOTTOM].normal.x = combinedMatrix._14 + combinedMatrix._12;
	planes_[PI_BOTTOM].normal.y = combinedMatrix._24 + combinedMatrix._22;
	planes_[PI_BOTTOM].normal.z = combinedMatrix._34 + combinedMatrix._32;
	planes_[PI_BOTTOM].distance = combinedMatrix._44 + combinedMatrix._42;

	// Top clipping plane
	planes_[PI_TOP].normal.x = combinedMatrix._14 - combinedMatrix._12;
	planes_[PI_TOP].normal.y = combinedMatrix._24 - combinedMatrix._22;
	planes_[PI_TOP].normal.z = combinedMatrix._34 - combinedMatrix._32;
	planes_[PI_TOP].distance = combinedMatrix._44 - combinedMatrix._42;

	// Near clipping plane
	planes_[PI_NEAR].normal.x = combinedMatrix._14 + combinedMatrix._13;
	planes_[PI_NEAR].normal.y = combinedMatrix._24 + combinedMatrix._23;
	planes_[PI_NEAR].normal.z = combinedMatrix._34 + combinedMatrix._33;
	planes_[PI_NEAR].distance = combinedMatrix._44 + combinedMatrix._43;

	// Far clipping plane
	planes_[PI_FAR].normal.x = combinedMatrix._14 - combinedMatrix._13;
	planes_[PI_FAR].normal.y = combinedMatrix._24 - combinedMatrix._23;
	planes_[PI_FAR].normal.z = combinedMatrix._34 - combinedMatrix._33;
	planes_[PI_FAR].distance = combinedMatrix._44 - combinedMatrix._43;

	for(unsigned int i=0; i<6; i++)
		planes_[i].normalize();
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
