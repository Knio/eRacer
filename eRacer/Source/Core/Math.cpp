#include "Math.h"

#include <cassert>


void debug(const Matrix &m)
{
	printf(
"\
_11 = %10.4f, _12 = %10.4f _13 = %10.4f _14 = %10.4f\n\
_21 = %10.4f, _22 = %10.4f _23 = %10.4f _24 = %10.4f\n\
_31 = %10.4f, _32 = %10.4f _33 = %10.4f _34 = %10.4f\n\
_41 = %10.4f, _42 = %10.4f _43 = %10.4f _44 = %10.4f\n\
\n",
		m._11, m._12, m._13, m._14, 
		m._21, m._22, m._23, m._24, 
		m._31, m._32, m._33, m._34, 
		m._41, m._42, m._43, m._44);
		
}
void debug(const Point3 &m){
	printf("(%6.2f %6.2f %6.2f)\n", m.x, m.y, m.z);
}

ostream& operator<<(ostream& s, const Vector3& v){
	return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}



bool affine(const Matrix& m)
{
	if (fabs(m._14) > ZERO) return false;
	if (fabs(m._24) > ZERO) return false;
	if (fabs(m._34) > ZERO) return false;
	if (fabs(m._44-1)>ZERO) return false;
	return true;
}

float abs(const Vector3 &A)
{
	return sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
}

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
	//cout << "normalized: " << A << endl;
	return A; ///= length(A);
}

Vector3 project(const Vector3 &a, const Vector3 &b)
{
	return b * (dot(a,b) / dot(b,b));
}
Vector3 projectOnto(const Vector3 &v, const Vector3 &normal)
{
	return v - normal*dot(v, normal);
}


Vector3 normalized(const Vector3& A){
	Vector3 result;
	D3DXVec3Normalize(&result, &A);
	return result;
}


Vector3 mul0(const Matrix &m, const Vector3 &v)
{
	//Make sure the matrix is affine
	assert(affine(m));
	Vector3 r;
	D3DXVec3TransformNormal(&r, &v, &m);
	return r;
}

Point3	mul1(const Matrix &m, const Point3  &v)
{
	//Make sure the matrix is affine
	assert(affine(m));

	Vector4 t;
	D3DXVec3Transform(&t, &v, &m);

	Vector3 r;
	memcpy(&r, &t, sizeof(Vector3));
	return r;
}


Matrix CreateMatrix(const Point3& position, const Matrix& orientation)
{
	Matrix r = orientation;
	r._41 = position.x;
	r._42 = position.y;
	r._43 = position.z;
	return r;
}


Matrix CreateMatrix(const Point3& position, float scaleX, float scaleY, float scaleZ){
	Matrix result(	scaleX, 0, 0, 0,
									0, scaleY, 0, 0,
									0, 0, scaleZ, 0,
									position.x, position.y, position.z, 1);
	return result;
}



Matrix CreateMatrix(const Point3& position, float s){
	Matrix result(	s, 0, 0, 0,
					0, s, 0, 0,
					0, 0, s, 0,
					position.x, position.y, position.z, 1);
	return result;
}

Matrix CreateMatrix(const Point3& position, float angle, const Vector3& axis, float scaleX, float scaleY, float scaleZ){
	Matrix m1;
	Matrix m2;
	Matrix result;
	D3DXMatrixScaling(&result,scaleX,scaleY,scaleZ);
	D3DXMatrixRotationAxis(&m2, &axis,angle);
	D3DXMatrixMultiply(&m1, &result, &m2);
	D3DXMatrixTranslation(&m2, position.x,position.y,position.z); 
	return *D3DXMatrixMultiply(&result, &m1, &m2);
}

Matrix CreateMatrix(const Point3& position, const Vector3& up, const Vector3& fw, float uniformScale)
{
	Vector3 ap = position;
	Vector3 az = normalized(fw);      
	Vector3 ax = normalized(cross(up, az));
	Vector3 ay = cross(az, ax);
	
	Matrix scalingMatrix;
	D3DXMatrixScaling(&scalingMatrix,uniformScale,uniformScale,uniformScale);
	
	return scalingMatrix * Matrix(
		ax.x, ax.y, ax.z,   0,
		ay.x, ay.y, ay.z,   0,
		az.x, az.y, az.z,   0,
		ap.x, ap.y, ap.z,   1
	);
}


Matrix CreateMatrix(float scaleX, float scaleY, float scaleZ){
	Matrix result;
	D3DXMatrixScaling(&result,scaleX,scaleY,scaleZ);
	return result;
}


Point3 ExtractPosition(const Matrix& matrix, Point3* position){
	Point3 pos;
	pos.x = matrix._41;
	pos.y = matrix._42;
	pos.z = matrix._43;
	if (position) *position = pos;
	return pos;
}

void ExtractScaling(const Matrix& matrix, float& scaleX, float& scaleY, float& scaleZ){
	Vector3 scale;
	Vector3 translation;
	D3DXQUATERNION rotation;
	assert(SUCCEEDED(D3DXMatrixDecompose(&scale,&rotation,&translation, &matrix)));
	scaleX = scale.x;
	scaleY = scale.y;
	scaleZ = scale.z;
}
void ExtractRotation(const Matrix& matrix, Matrix& rotationMatrix){
	Vector3 scaleOrAxis;
	Vector3 translation;
	D3DXQUATERNION rotation;
	float angle;
	assert(SUCCEEDED(D3DXMatrixDecompose(&scaleOrAxis,&rotation,&translation, &matrix)));
	D3DXQuaternionToAxisAngle(&rotation,&scaleOrAxis, &angle);
	D3DXMatrixRotationAxis(&rotationMatrix, &scaleOrAxis,angle);
}


void ExtractAngleAxis(const Matrix& matrix, float& angle, Vector3& axis){
	Vector3 scale;
	Vector3 translation;
	D3DXQUATERNION rotation;
	assert(SUCCEEDED(D3DXMatrixDecompose(&scale,&rotation,&translation, &matrix)));
	D3DXQuaternionToAxisAngle(&rotation,&axis, &angle);
}



void Decompose(const Matrix& matrix, Point3& position, Matrix& rotation, float& scaleX, float& scaleY, float& scaleZ){
	Vector3 scaleOrAxis;
	D3DXQUATERNION quat;
	
	float angle;
	assert(SUCCEEDED(D3DXMatrixDecompose(&scaleOrAxis,&quat,&position, &matrix)));
	scaleX = scaleOrAxis.x;
	scaleY = scaleOrAxis.y;
	scaleZ = scaleOrAxis.z;

	D3DXQuaternionToAxisAngle(&quat,&scaleOrAxis, &angle);
	D3DXMatrixRotationAxis(&rotation, &scaleOrAxis,angle);
}

Matrix& SetTranslation(Matrix& matrix, const Vector3& translation){
	matrix._41 = translation.x;
	matrix._42 = translation.y;
	matrix._43 = translation.z;
	return matrix;
}

Matrix& Translate(Matrix& matrix, const Vector3& translation){
	matrix._41 += translation.x;
	matrix._42 += translation.y;
	matrix._43 += translation.z;
	return matrix;
}

Matrix Translated(const Matrix& matrix, const Vector3& translation){
	Matrix result(matrix);
	return Translate(result,translation);
}

Vector3 GetTranslation(const Matrix& matrix){
	return Vector3(matrix._41, matrix._42, matrix._43);
}

/*
Matrix& Rotate(Matrix& matrix, float yaw, float pitch, float roll){
	Matrix m2;
	D3DXMatrixRotationYawPitchRoll(&m2, yaw, pitch, roll);
	return matrix*=m2;
}

Matrix Rotated(const Matrix& matrix, float yaw, float pitch, float roll){
	Matrix result(matrix);
	Rotate(result,yaw,pitch,roll);
	return result;
}
*/

Matrix Inverse(const Matrix& m)
{
	Matrix r;
	D3DXMatrixInverse(&r, NULL, &m);
	return r;
}

Matrix Scaled(const Matrix& matrix, float x, float y, float z){
	Matrix result;
	Matrix scaling;
	D3DXMatrixScaling(&scaling, x,y,z);
	return result = scaling*matrix;
}

