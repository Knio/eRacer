
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


Vector3 transformAffine(const Matrix& T, const Vector3& u);



Matrix CreateMatrix(const Point3& position=ORIGIN, const Matrix& orientation=IDENTITY);

#endif
