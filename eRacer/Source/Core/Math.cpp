#include "Math.h"

#include <cassert>


Vector3 cross(const Vector3 &A, const Vector3 &B)
{
	Vector3 C;
	D3DXVec3Cross(&C, &A, &B);
	return C;
}

float dot(const Vector3 &A, const Vector3 &B)
{
	return D3DXVec3Dot(&A, &B);
}

Vector3& normalize(Vector3& A){
	D3DXVec3Normalize(&A, &A);
	return A;
}


Vector3 normalized(const Vector3& A){
	Vector3 result;
	D3DXVec3Normalize(&result, &A);
	return result;
}


Vector3 transformAffine(const Matrix& T, const Vector3& u){
	//Make sure the matrix is affine
	assert(0 == T._41);
	assert(0 == T._42);
	assert(0 == T._43);
	assert(1 == T._44);

	Vector4 temp;
	D3DXVec3Transform(&temp, &u, &T);

	Vector3 result;
	memcpy(&result, &temp, sizeof(Vector3));
	return result;

}
