
#ifndef VECT3_H_
#define VECT3_H_

#include <d3dx9math.h>

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXVECTOR3 Point3;
typedef D3DXVECTOR4 Point4;
typedef D3DXMATRIX Matrix;


const Point3 ORIGIN(0,0,0); 
const Vector3 X(1,0,0); 
const Vector3 Y(0,1,0); 
const Vector3 Z(0,0,1);

const Matrix IDENTITY(1,0,0,0, 
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1);


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

#endif
