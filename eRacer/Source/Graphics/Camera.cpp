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
	Point3 a, b, c;

	a = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_TOP_NEAR]);
	b = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_BOTTOM_NEAR]);
	c = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_BOTTOM_FAR]);
	planes_[PI_LEFT].recompute(a,b,c);

	a = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_TOP_NEAR]);
	b = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_FAR]);
	c = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_NEAR]);
	planes_[PI_RIGHT].recompute(a,b,c);

	a = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_BOTTOM_NEAR]);
	b = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_NEAR]);
	c = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_FAR]);
	planes_[PI_BOTTOM].recompute(a,b,c);

	a = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_TOP_NEAR]);
	b = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_TOP_FAR]);
	c = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_TOP_NEAR]);
	planes_[PI_TOP].recompute(a,b,c);

	a = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_TOP_NEAR]);
	b = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_TOP_NEAR]);
	c = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_NEAR]);
	planes_[PI_NEAR].recompute(a,b,c);

	a = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_BOTTOM_FAR]);
	b = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_FAR]);
	c = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_TOP_FAR]);
	planes_[PI_FAR].recompute(a,b,c);



	//TODO - should have happende already
	//UpdateProjection();
	//UpdateView();




	/*
	Matrix m = viewMatrix_*projectionMatrix_;
   
	// Left clipping plane
	planes_[PI_LEFT].normal.x = m._14 + m._11;
	planes_[PI_LEFT].normal.y = m._24 + m._21;
	planes_[PI_LEFT].normal.z = m._34 + m._31;
	planes_[PI_LEFT].distance = m._44 + m._41;

	// Right clipping plane
	planes_[PI_RIGHT].normal.x = m._14 - m._11;
	planes_[PI_RIGHT].normal.y = m._24 - m._21;
	planes_[PI_RIGHT].normal.z = m._34 - m._31;
	planes_[PI_RIGHT].distance = m._44 - m._41;

	// Bottom clipping plane
	planes_[PI_BOTTOM].normal.x = m._14 + m._12;
	planes_[PI_BOTTOM].normal.y = m._24 + m._22;
	planes_[PI_BOTTOM].normal.z = m._34 + m._32;
	planes_[PI_BOTTOM].distance = m._44 + m._42;

	// Top clipping plane
	planes_[PI_TOP].normal.x = m._14 - m._12;
	planes_[PI_TOP].normal.y = m._24 - m._22;
	planes_[PI_TOP].normal.z = m._34 - m._32;
	planes_[PI_TOP].distance = m._44 - m._42;

	// Near clipping plane
	planes_[PI_NEAR].normal.x = m._13;
	planes_[PI_NEAR].normal.y = m._23;
	planes_[PI_NEAR].normal.z = m._33;
	planes_[PI_NEAR].distance = -m._43;

	// Far clipping plane
	// planes_[PI_FAR].normal.x = m._14 - m._13;
	// planes_[PI_FAR].normal.y = m._24 - m._23;
	// planes_[PI_FAR].normal.z = m._34 - m._33;
	// planes_[PI_FAR].distance = -(m._44 - m._43);
	planes_[PI_FAR].normal.x = -planes_[PI_NEAR].normal.x;
	planes_[PI_FAR].normal.y = -planes_[PI_NEAR].normal.y;
	planes_[PI_FAR].normal.z = -planes_[PI_NEAR].normal.z;
	// planes_[PI_FAR].distance = -planes_[PI_NEAR].distance-far_;
	

	for(unsigned int i=0; i<PI_NUM; i++)
		planes_[i].normalize();
	//static long long counter = 0;
	//cout << "recomputed"<< counter++ << endl;
	*/
}

const Plane& Camera::GetPlane(int planeIndex) const {
	assert(0 <= planeIndex);
	assert(planeIndex < PI_NUM);
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
	float dxNear, dyNear, dxFar, dyFar;
	if(perspective_){
		D3DXMatrixPerspectiveFovLH(&projectionMatrix_,
	                              fovY_,
	                              aspectRatio_,
	                              near_,
	                              far_ );

		float sinFovX = sin(fovY_*aspectRatio_);
		float sinFovY = sin(fovY_);

		dxNear = near_/ sinFovX;
		dyNear = near_/ sinFovY;


		dxFar = far_ / sinFovX;
		dyFar = far_ / sinFovY;
	} else{
		D3DXMatrixOrthoLH(&projectionMatrix_,
	                              height_*aspectRatio_,
		                            height_,
	                              near_,
	                              far_ );

		dxNear = height_*aspectRatio_/2.0f;
		dyNear = height_/2.0f;

		dxFar = dxNear;
		dyFar = dyNear;

	}

	frustumCorners_[CI_LEFT_BOTTOM_NEAR].x = -dxNear; 
	frustumCorners_[CI_LEFT_BOTTOM_NEAR].y = -dyNear;
	frustumCorners_[CI_LEFT_BOTTOM_NEAR].z = near_;

	frustumCorners_[CI_RIGHT_BOTTOM_NEAR].x = dxNear; 
	frustumCorners_[CI_RIGHT_BOTTOM_NEAR].y = -dyNear;
	frustumCorners_[CI_RIGHT_BOTTOM_NEAR].z = near_;

	frustumCorners_[CI_LEFT_TOP_NEAR].x = -dxNear; 
	frustumCorners_[CI_LEFT_TOP_NEAR].y = dyNear;
	frustumCorners_[CI_LEFT_TOP_NEAR].z = near_;

	frustumCorners_[CI_RIGHT_TOP_NEAR].x = dxNear; 
	frustumCorners_[CI_RIGHT_TOP_NEAR].y = dyNear;
	frustumCorners_[CI_RIGHT_TOP_NEAR].z = near_;

	frustumCorners_[CI_LEFT_BOTTOM_FAR].x = -dxFar; 
	frustumCorners_[CI_LEFT_BOTTOM_FAR].y = -dyFar;
	frustumCorners_[CI_LEFT_BOTTOM_FAR].z = far_;

	frustumCorners_[CI_RIGHT_BOTTOM_FAR].x = dxFar; 
	frustumCorners_[CI_RIGHT_BOTTOM_FAR].y = -dyFar;
	frustumCorners_[CI_RIGHT_BOTTOM_FAR].z = far_;

	frustumCorners_[CI_LEFT_TOP_FAR].x = -dxFar; 
	frustumCorners_[CI_LEFT_TOP_FAR].y = dyFar;
	frustumCorners_[CI_LEFT_TOP_FAR].z = far_;

	frustumCorners_[CI_RIGHT_TOP_FAR].x = dxFar; 
	frustumCorners_[CI_RIGHT_TOP_FAR].y = dyFar;
	frustumCorners_[CI_RIGHT_TOP_FAR].z = far_;
}


}
