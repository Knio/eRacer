/**
 * @file Camera.h
 * @brief
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "../Core/types.h"



/**
 * @brief
 */
class Camera {
public:
	Camera(const Point3& position = ORIGIN, const Point3& lookAt = Point3(0,0,1), const Vector3& approxUp=Vector3(0,1,0));
	~Camera();

	void setFrame(const Point3& position, const Point3& lookAt, const Vector3& approxUp);

	void setPosition(const Point3& position);
	void setLookAt(const Point3& lookAt);
	void setUp(const Vector3& approxUp);

	void setNear(float near);
	void setFar(float far);
	void setAspectRatio(float aspectRatio);
	void setFovY(float fovY);

	const Matrix& getViewMatrix() const;
	const Matrix& getProjectionMatrix() const;
private:
	void updateView();
	void updateProjection();

	Matrix viewMatrix_;
	Matrix projectionMatrix_;

	Point3 position_;
	Point3 lookAt_;
	Vector3 up_;

	float near_;
	float far_;
	float aspectRatio_;
	float fovY_;

	Plane planes_[6];
};

inline void Camera::setFrame(const Point3& position, const Point3& lookAt, const Vector3& approxUp){
	position_ = position;
	lookAt_ = lookAt;
	setUp(approxUp);

	updateView();
}


inline void Camera::setPosition(const Point3& position){
	position_ = position;

	//adjust up so that it is perpendicular to view direction
	setUp(up_);

	updateView();
}

inline void Camera::setLookAt(const Point3& lookAt){
	lookAt_ = lookAt;

	//adjust up so that it is perpendicular to view direction
	setUp(up_);

	updateView();
}

inline void Camera::setUp(const Vector3& approxUp){
	up_ = approxUp;
	Vector3 viewDirection = lookAt_ - position_;
	D3DXVec3Normalize(&viewDirection,&viewDirection);
	D3DXVec3Normalize(&up_, &up_);
	D3DXVec3Cross(&up_,&viewDirection, &approxUp);
	D3DXVec3Cross(&up_,&up_, &viewDirection);
}

inline void Camera::updateView(){
	D3DXMatrixLookAtLH(&viewMatrix_,
	                      &position_,
	                      &lookAt_,
	                      &up_);

}

inline void Camera::setNear(float near){
	assert(near>0);
	near_ = near;
}

inline void Camera::setFar(float far){
	assert(far>0);
	assert(far>=near);
	far_ = far;
}

inline void Camera::setAspectRatio(float aspectRatio){
	assert(aspectRatio>0);
	aspectRatio_ = aspectRatio;
}

inline void Camera::setFovY(float fovY){
	assert(fovY>0);
	assert(fovY<PI);
	fovY_ = fovY;
}


inline void Camera::updateProjection(){
	   D3DXMatrixPerspectiveFovLH(&projectionMatrix_,
	                              fovY_,
	                              aspectRatio_,
	                              near_,
	                              far_ );
}

inline const Matrix& Camera::getViewMatrix() const{
	return viewMatrix_;
}
inline const Matrix& Camera::getProjectionMatrix() const{
	return projectionMatrix_;
}



#endif /* CAMERA_H_ */
