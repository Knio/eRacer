/**
 * @file Camera.h
 * @brief Definition of the Camera class
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "../Core/types.h"
#include <cassert>

namespace Graphics {

/**
 * @brief A camera, defining position, direction, up vector and the view frustum.
 */
class Camera {
public:
	/**
	 * @brief Constructor. Create a new camera.
	 *
	 * The near plane will be set to a distance of 1, the far plane at a distance of 100.
	 * The aspect ratio will be set to 4:3 (.75) and the field of view in y direction to
	 * 45 degrees (PI/4 radiants).
	 *
	 * @param position
	 * 			the eye point of the camera - default: (0,0,0)
	 * @param lookAt
	 * 			the point the camera should point at - default: (0,0,1)
	 * @param up
	 * 			a vector that approximates the up direction. The actual up vector
	 * 			will be the closest vector that is perpedicular to the view direction
	 * 			- default: (0,1,0)
	 */
	Camera(const Point3& position = ORIGIN, const Point3& lookAt = Point3(0,0,1), const Vector3& approxUp=Vector3(0,1,0));

	/**
	 * @brief Destructor stub.
	 */
	~Camera();

	/**
	 * @brief Set position, lookAt and up vector at once.
	 *
	 * This is useful for better performance, as the perpendicular up vector has to be
	 * computed only once.
	 *
	 * @param position
	 * 			the eye point of the camera
	 * @param lookAt
	 * 			the point the camera should point at
	 * @param up
	 * 			a vector that approximates the up direction. The actual up vector
	 * 			will be the closest vector that is perpedicular to the view direction
	 */
	void setFrame(const Point3& position, const Point3& lookAt, const Vector3& approxUp);

	/**
	 * @brief set position
	 *
	 * This method will trigger recomputation of the up vector to make sure it is still
	 * perpendicular to the view direction.
	 *
	 * @param position
	 * 			the eye point of the camera
	 */
	void setPosition(const Point3& position);

	/**
	 * @brief set point where the camera should look at
	 *
	 * This method will trigger recomputation of the up vector to make sure it is still
	 * perpendicular to the view direction.
	 *
	 * @param lookAt
	 * 			the point the camera should point at
	 */
	void setLookAt(const Point3& lookAt);

	/**
	 * @brief set the up vector
	 *
	 * It is sufficient to specify an approximation of the up vector. This method will
	 * chose the vector closest to approxUp that is perpendicular to the view direction.
	 *
	 * @param approxUp
	 * 			an approximation of the up vector
	 */
	void setUp(const Vector3& approxUp);

	/**
	 * @brief set distance to near plane
	 *
	 * The actual near plane is the plane which is perpendicular to the view direction
	 * and has the specified minimal distance to the eye point.
	 *
	 * @param near
	 * 			the distance to the near plane - must be positive
	 */
	void setNear(float near);

	/**
	 * @brief set the distance to the far plane
	 *
	 * The actual far plane is the plane which is perpendicular to the view direction
	 * and has the specified minimal distance to the eye point.
	 *
	 * @param far
	 * 			the distance to the far plane - must be positive and greater than near
	 */
	void setFar(float far);

	/**
	 * @brief set the aspect ratio (height/width)
	 *
	 * @param aspectRatio
	 * 			the ratio between the height and the width of the view frustum - must
	 * 			be positive
	 */
	void setAspectRatio(float aspectRatio);

	/**
	 * @brief set the field of view in y direction in radiants
	 *
	 * @param fovY
	 * 			the field of view in y direction in radiants - must be positive and
	 * 			smaller than PI (180 degrees)
	 */
	void setFovY(float fovY);

	/**
	 * @brief get the view matrix
	 *
	 * It is guaranteed that the view matrix is always current.
	 *
	 * @return the view matrix
	 */
	const Matrix& getViewMatrix() const;

	/**
	 * @brief get the projection matrix
	 *
	 * It is guaranteed that the projection matrix is always current.
	 *
	 * @return the projection matrix
	 */
	const Matrix& getProjectionMatrix() const;

	/**
	 * @brief get the specified plane limiting the view frustum
	 *
	 * The indices are as follows:
	 * 0 - left
	 * 1 - right
	 * 2 - bottom
	 * 3 - top
	 * 4 - near
	 * 5 - far
	 *
	 * @param planeIndex
	 * 			The index of the plane to return
	 * @return the specified plane
	 */
	const Plane& getPlane(int planeIndex);
private:
	void updateView();
	void updateProjection();
	void updatePlanes();

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
	normalize(viewDirection);
	normalize(up_);
	up_ = cross(cross(viewDirection, approxUp),viewDirection);
}

inline void Camera::updateView(){
	D3DXMatrixLookAtLH(&viewMatrix_,
	                      &position_,
	                      &lookAt_,
	                      &up_);

}


inline void Camera::setNear(float nea){
	assert(nea>0);
	near_ = nea;
}

inline void Camera::setFar(float fa){
	assert(fa>0);
	assert(fa>=near_);
	far_ = fa;
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

}

#endif /* CAMERA_H_ */
