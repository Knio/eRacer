#include "vect3.h"

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
