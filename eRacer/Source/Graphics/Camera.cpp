/**
 * @file Camera.cpp
 * @brief
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#include "Camera.h"
#include "GraphicsLayer.h"
#include "DebugRenderable.h"

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
	Point3 
		ltn,
		lbn,
		rtn,
		rbn,
		ltf,
		lbf,
		rtf,
		rbf;

	ltn = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_TOP_NEAR]);
	ltf = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_TOP_FAR]);
	lbn = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_BOTTOM_NEAR]);
	lbf = mul1(inverseViewMatrix_, frustumCorners_[CI_LEFT_BOTTOM_FAR]);
	
	rtn = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_TOP_NEAR]);
  rtf = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_TOP_FAR]);
	rbf = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_FAR]);
	rbn = mul1(inverseViewMatrix_, frustumCorners_[CI_RIGHT_BOTTOM_NEAR]);
	
	planes_[PI_LEFT  ].recompute(ltn,lbn,lbf);
	planes_[PI_RIGHT ].recompute(rtn,rbf,rbn);
	planes_[PI_BOTTOM].recompute(lbn,rbn,rbf);
	planes_[PI_TOP   ].recompute(ltn,rtf,rtn);
	planes_[PI_NEAR  ].recompute(ltn,rtn,rbn);
	planes_[PI_FAR   ].recompute(lbf,rbf,ltf);
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

		float tanFovY = tan(fovY_ / 2.f);
		float tanFovX = tan(fovY_ / 2.f * aspectRatio_);

		dxNear = near_ * tanFovX;
		dyNear = near_ * tanFovY;


		dxFar = far_ * tanFovX;
		dyFar = far_ * tanFovY;
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

	frustumCorners_[CI_RIGHT_BOTTOM_NEAR].x = +dxNear; 
	frustumCorners_[CI_RIGHT_BOTTOM_NEAR].y = -dyNear;
	frustumCorners_[CI_RIGHT_BOTTOM_NEAR].z = near_;

	frustumCorners_[CI_LEFT_TOP_NEAR].x = -dxNear; 
	frustumCorners_[CI_LEFT_TOP_NEAR].y = +dyNear;
	frustumCorners_[CI_LEFT_TOP_NEAR].z = near_;

	frustumCorners_[CI_RIGHT_TOP_NEAR].x = +dxNear; 
	frustumCorners_[CI_RIGHT_TOP_NEAR].y = +dyNear;
	frustumCorners_[CI_RIGHT_TOP_NEAR].z = near_;

	frustumCorners_[CI_LEFT_BOTTOM_FAR].x = -dxFar; 
	frustumCorners_[CI_LEFT_BOTTOM_FAR].y = -dyFar;
	frustumCorners_[CI_LEFT_BOTTOM_FAR].z = far_;

	frustumCorners_[CI_RIGHT_BOTTOM_FAR].x = +dxFar; 
	frustumCorners_[CI_RIGHT_BOTTOM_FAR].y = -dyFar;
	frustumCorners_[CI_RIGHT_BOTTOM_FAR].z = far_;

	frustumCorners_[CI_LEFT_TOP_FAR].x = -dxFar; 
	frustumCorners_[CI_LEFT_TOP_FAR].y = +dyFar;
	frustumCorners_[CI_LEFT_TOP_FAR].z = far_;

	frustumCorners_[CI_RIGHT_TOP_FAR].x = +dxFar; 
	frustumCorners_[CI_RIGHT_TOP_FAR].y = +dyFar;
	frustumCorners_[CI_RIGHT_TOP_FAR].z = far_;
	
	
}


}
