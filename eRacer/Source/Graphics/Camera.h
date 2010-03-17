/**
 * @file Camera.h
 * @brief Definition of the Camera class
 *
 * @date 14.01.2010
 * @author Ole Rehmsen
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Core/types.h"
#include <cassert>

namespace Graphics {

/**
 * @brief An enumeration to refer to the frustum planes in the planes array
 */
enum PlaneIndex {
	PI_LEFT,
	PI_RIGHT,
	PI_BOTTOM,
	PI_TOP,
	PI_NEAR,
	PI_FAR,
	PI_NUM
};

/**
 * @brief An enumeration to refer to the frustum planes in the planes array
 */
enum CornerIndex {
	CI_LEFT_BOTTOM_NEAR,
	CI_RIGHT_BOTTOM_NEAR,
	CI_LEFT_TOP_NEAR,
	CI_RIGHT_TOP_NEAR,
	CI_LEFT_BOTTOM_FAR,
	CI_RIGHT_BOTTOM_FAR,
	CI_LEFT_TOP_FAR,
	CI_RIGHT_TOP_FAR,
	CI_NUM
};

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
	 * 			the eye point of the camera - default: (0,0,-1)
	 * @param lookAt
	 * 			the point the camera should point at - default: (0,0,0)
	 * @param approxUp
	 * 			a vector that approximates the up direction. The actual up vector
	 * 			will be the closest vector that is perpedicular to the view direction
	 * 			- default: (0,1,0)
	 * @param perspective
	 *			true for perspective projection, false for orthographic
	 */
	Camera(const Point3& position = -Z, const Point3& lookAt = ORIGIN, const Vector3& approxUp=Y, bool perspective = true);

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
	 * 			the eye point of the camera - default: (0,0,-1)
	 * @param lookAt
	 * 			the point the camera should point at - default: (0,0,0)
	 * @param approxUp
	 * 			a vector that approximates the up direction. The actual up vector
	 * 			will be the closest vector that is perpedicular to the view direction
	 *			- default: (0,1,0)
	 * @param perspective
	 *			true for perspective projection, false for orthographic
	 */
	void SetFrame(const Point3& position=-Z, const Point3& lookAt=ORIGIN, const Vector3& approxUp=Y, bool perspective = TRUE);

	/**
	 * @brief get the position of the camera's eye point
	 * @return the camera's  eye point
	 */
	const Point3& GetPosition() const;

	/**
	 * @brief set position
	 *
	 * This method will trigger recomputation of the up vector to make sure it is still
	 * perpendicular to the view direction.
	 *
	 * @param position
	 * 			the eye point of the camera
	 */
	void SetPosition(const Point3& position);

	/**
	 * @brief get a point that the camera is looking at 
	 *
	 * This point is not unique! This method returns a point on the line starting
	 * at the eye point and pointing in direction of the cameras direction.
	 * @return a point the camera is looking straight at
	 */
	const Point3& GetLookAt() const;


	/**
	 * @brief set point where the camera should look at
	 *
	 * This method will trigger recomputation of the up vector to make sure it is still
	 * perpendicular to the view direction.
	 *
	 * @param lookAt
	 * 			the point the camera should point at
	 */
	void SetLookAt(const Point3& lookAt);

	/**
	 * @brief getter for the up vector
	 *
	 * @return the camera's up vector
	 */
	const Vector3& GetUp() const;

	/**
	 * @brief set the up vector
	 *
	 * It is sufficient to specify an approximation of the up vector. This method will
	 * chose the vector closest to approxUp that is perpendicular to the view direction.
	 *
	 * @param approxUp
	 * 			an approximation of the up vector
	 */
	void SetUp(const Vector3& approxUp);

	/**
	 * @brief set distance to near plane
	 *
	 * The actual near plane is the plane which is perpendicular to the view direction
	 * and has the specified minimal distance to the eye point.
	 *
	 * @param near
	 * 			the distance to the near plane - must be positive
	 */
	void SetNear(float near);

	/**
	 * @brief Getter for the distance of the far plane
	 * @return the distance from the camera position to the far plane
	 */
	float GetFar() const;

	/**
	 * @brief set the distance to the far plane
	 *
	 * The actual far plane is the plane which is perpendicular to the view direction
	 * and has the specified minimal distance to the eye point.
	 *
	 * @param far
	 * 			the distance to the far plane - must be positive and greater than near
	 */
	void SetFar(float far);

	/**
	 * @brief set the aspect ratio (width/height)
	 *
	 * @param aspectRatio
	 * 			the ratio between the width and the height of the view frustum - must
	 * 			be positive
	 */
	void SetAspectRatio(float aspectRatio);

	/**
	 * @brief set the field of view in y direction in radiants
	 *
	 * This method may only be called if using perspective projection!
	 * The field of view in x direction is deducted using the aspect ratio
	 *
	 * @param fovY
	 * 			the field of view in y direction in radiants - must be positive and
	 * 			smaller than PI (180 degrees)
	 * @see SetWidth
	 * @see SetAspectRatio
	 */
	void SetFovY(float fovY);

	/**
	 * @brief set the height of the viewing volume
	 *
	 * This method may only be called if using orthographic projection!
	 * The width is deducted using the aspect ratio
	 *
	 * @param height
	 * 			the height of the viewing volume - must be positive
	 * @see SetFovY
	 * @see SetAspectRatio
	 */
	void SetHeight(float height);

	/**
	 * @brief get the view matrix
	 *
	 * It is guaranteed that the view matrix is always current.
	 *
	 * @return the view matrix
	 */
	const Matrix& GetViewMatrix() const;

	/**
	 * @brief Setter for the view matrix
	 *
	 * Position, look at point and up vector are computed accordingly
	 *
	 * @brief viewMatrix the new view matrix to use
	 */
	void SetViewMatrix(const Matrix& viewMatrix);
	
	/**
	 * @brief get the projection matrix
	 *
	 * It is guaranteed that the projection matrix is always current.
	 *
	 * @return the projection matrix
	 */
	const Matrix& GetProjectionMatrix() const;

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
	 * @see PlaneIndex
	 */
	const Plane& GetPlane(int planeIndex) const;
	

	

protected:
	void UpdateView();
	void UpdateProjection();
	void UpdatePlanes();
	void UpdateUp();

	Matrix viewMatrix_;
	Matrix projectionMatrix_;
	Matrix inverseViewMatrix_;


	Point3 position_;
	Point3 lookAt_;
	Vector3 approxUp_;
	Vector3 up_;
	
	float near_;
	float far_;
	float aspectRatio_;
	float fovY_;
	float height_;

	bool perspective_;

	Point3 frustumCorners_[8];
	Plane planes_[6];
};


inline void Camera::UpdateView(){
	D3DXMatrixLookAtLH(&viewMatrix_,
	                      &position_,
	                      &lookAt_,
	                      &up_);
	D3DXMatrixInverse(&inverseViewMatrix_, NULL, &viewMatrix_);
}


inline void Camera::SetNear(float nea){
	assert(nea>0);
	near_ = nea;
}

inline float Camera::GetFar() const{
	return far_;
}


inline void Camera::SetFar(float fa){
	assert(fa>0);
	assert(fa>=near_);
	far_ = fa;
}



inline const Matrix& Camera::GetViewMatrix() const{
	return viewMatrix_;
}
inline const Matrix& Camera::GetProjectionMatrix() const{
	return projectionMatrix_;
}

inline const Point3& Camera::GetPosition() const { 
	return position_; 
}

inline const Point3& Camera::GetLookAt() const { 
	return lookAt_; 
}

inline const Vector3& Camera::GetUp() const { 
	return up_; 
}


};

#endif /* CAMERA_H_ */
