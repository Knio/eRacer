
#ifndef MATH_H_
#define MATH_H_

#include <d3dx9math.h>
#include <limits>
#include <cfloat>

#define PI D3DX_PI
#define DEG2RAD(x) (PI*(x)/180.0)
#define RAD2DEG(x) (180.0*(x)/PI)

const float INF   = std::numeric_limits<float>::infinity();
const float NaN   = std::numeric_limits<float>::quiet_NaN();

template <typename T>
bool isnan(T x) { return x != x; }


typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXVECTOR3 Point3;
typedef D3DXVECTOR4 Point4;
typedef D3DXMATRIX  Matrix;


const Point3 ORIGIN = Point3(0,0,0); 
const Vector3 X = Vector3(1,0,0); 
const Vector3 Y = Vector3(0,1,0); 
const Vector3 Z = Vector3(0,0,1);

const Matrix IDENTITY = Matrix(1,0,0,0, 
							   0,1,0,0,
							   0,0,1,0,
							   0,0,0,1);


#pragma warning( disable : 4996 4995 ) // disable deprecated warning 
#include <strsafe.h>

void debug(Matrix &m);

float abs(const Vector3 &A);
Vector3 cross(const Vector3 &A, const Vector3 &B);
float dot(const Vector3 &A, const Vector3 &B);

/**
 * @brief normalize a vector in-place
 *
 * @param A
 * 			the vector to normalize
 * @return a reference to the vector - for chaining
 */
Vector3& normalize(Vector3& A);

/**
 * @brief create a normalized version of a vector
 *
 * @param A
 * 			the vector to normalize - will not be changed!
 * @return a new vector that is the normalized ve rsion of A
 */
Vector3 normalized(const Vector3& A);


inline float length(const Vector3& v){
	return D3DXVec3Length(&v);
}


Vector3 mul0(const Matrix &m, const Vector3 &v);
Point3	mul1(const Matrix &m, const Point3  &v);

Vector3 transformedAffine(const Matrix& T, const Vector3& u);
const Vector3& transformAffine(const Matrix& T, Vector3& u);



/**
 * @brief Construct a matrix from position and orientation
 *
 * @param position
 *			the translational component of the matrix - default: origin
 * @param orientation
 *			the rotational component of the matrix as a matrix - default: identity
 * @return the matrix TR, where T is the transation matrix and R the rotation matrix. This means R is applied first!
 */
Matrix CreateMatrix(const Point3& position=ORIGIN, const Matrix& orientation=IDENTITY);

/**
 * @brief Construct a matrix from position, angle, axis and scaling.
 *
 * @param position
 *			the translational component of the matrix
 * @param angle
 *			the angle by which to rotate
 * @param axis
 *			the axis around which we rotate
 * @param scaleX
 *			scaling factor in x direction - default: 1
 * @param scaleY
 *			scaling factor in y direction - default: 1
 * @param scaleZ
 *			scaling factor in z direction - default: 1
 * @return the matrix representing the transformation of first applying the scaling, 
 *			then the rotation and finally the translation
 */
Matrix CreateMatrix(const Point3& position, float angle, const Vector3& axis, float scaleX=1, float scaleY=1, float scaleZ=1);


/**
 * @brief Construct a matrix from position, yaw, pitch, roll and scaling.
 *
 * @param position
 *			the translational component of the matrix
 * @param yaw
 *			the angle by which to rotate around the y-axis
 * @param pitch
 *			the angle by which to rotate around the x-axis
 * @param roll
 *			the angle by which to rotate around the z-axis
 * @param scaleX
 *			scaling factor in x direction - default: 1
 * @param scaleY
 *			scaling factor in y direction - default: 1
 * @param scaleZ
 *			scaling factor in z direction - default: 1
 * @return the matrix representing the transformation of first applying the scaling, 
 *			then the rotation and finally the translation
 */
Matrix CreateMatrix(const Point3& position, float yaw, float pitch, float roll, float scaleX=1, float scaleY=1, float scaleZ=1);


Matrix CreateMatrix(float scaleX, float scaleY, float scaleZ);

/**
 * @brief Extract only the translational component from a transformation matrix.
 *
 * @param matrix
 *			the matrix to extract from
 * @param position
 *			out: the translational component
 */
void ExtractPosition(const Matrix& matrix, Point3& position);

/**
 * @brief Extract only the scaling component from a transformation matrix.
 *
 * @param matrix
 *			the matrix to extract from
 * @param scaleX
 *			out: the scaling factor in x direction
 * @param scaleY
 *			out: the scaling factor in y direction
 * @param scaleZ
 *			out: the scaling factor in z direction
 */
void ExtractScaling(const Matrix& matrix, float& scaleX, float& scaleY, float& scaleZ);

/**
 * @brief Extract only the rotational component as a matrix from a transformation matrix.
 *
 * @param matrix
 *			the matrix to extract from
 * @param rotation
 *			out: the rotational component as a matrix
 */
void ExtractRotation(const Matrix& matrix, Matrix& rotation);

/**
 * @brief Extract only the rotational component as angle/axis pair from a transformation matrix.
 *
 * @param matrix
 *			the matrix to extract from
 * @param angle
 *			out: the angle by which is rotated 
 * @param axis
 *			out: the axis around which is rotated
 */
void ExtractAngleAxis(const Matrix& matrix, float& angle, Vector3& axis);

/**
 * @brief Decompose a transformation matrix into a position, a rotation matrix and three scale factors
 *
 * @param matrix
 *			the matrix to decompose
 * @param position
 *			out: the translational component
 * @param rotation
 *			out: the rotational component as a matrix
 * @param scaleX
 *			out: the scaling factor in x direction
 * @param scaleY
 *			out: the scaling factor in y direction
 * @param scaleZ
 *			out: the scaling factor in z direction
 */
void Decompose(const Matrix& matrix, Point3& position, Matrix& rotation, float& scaleX, float& scaleY, float& scaleZ);


#endif
