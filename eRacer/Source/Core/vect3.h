
#ifndef VECT3_H_
#define VECT3_H_

#include <d3dx9math.h>

struct Vect3 : public D3DXVECTOR3
{
	Vect3()								: D3DXVECTOR3() {};
	Vect3( CONST FLOAT * v)				: D3DXVECTOR3(v) {}
	Vect3( CONST D3DVECTOR& v)			: D3DXVECTOR3(v) {}
	Vect3( CONST D3DXFLOAT16 * v)		: D3DXVECTOR3(v) {}
	Vect3( FLOAT x, FLOAT y, FLOAT z )	: D3DXVECTOR3(x, y, z) {}
	
	inline Vect3 cross(const Vect3 &B) const
	{
		Vect3 C;
		D3DXVec3Cross(&C, this, &B);
		return C;
	}
	inline float dot(const Vect3 &B) const
	{
		return D3DXVec3Dot(this, &B);
	}
	inline float abs() const
	{
	   return D3DXVec3Length(this);
	}
	
	inline float abssq() const
	{
	   return D3DXVec3LengthSq(this);
	}

	inline Vect3 normal() const
	{
		Vect3 B;
		D3DXVec3Normalize(&B, this);
		return B;
	}
	inline void normalize()
	{
		D3DXVec3Normalize(this, this);
	}
	// TODO more methods
};

struct Point3 : public Vect3
{	
	Point3()							: Vect3() {};
	Point3( CONST FLOAT * v)			: Vect3(v) {}
	Point3( CONST D3DVECTOR& v)			: Vect3(v) {}
	Point3( CONST D3DXFLOAT16 * v)		: Vect3(v) {}
	Point3( FLOAT x, FLOAT y, FLOAT z )	: Vect3(x, y, z) {}	
	
	/*
	empty, will only be used for doing different
	things when multiplying by a matrix

	Point + Point = Invalid
	Point - Point = Vector
	Point + Vector = Point
	Point - Vector = Point
	Vector + Vector = Vector
	Vector - Vector = Vector

	Is it worth it to enforce these rules in the type
	system? (probably not) or use a 4D homogeneous
	coordinate system?

	*/
};

struct Matrix : public D3DXMATRIX
{
	Matrix()						: D3DXMATRIX() {};
	Matrix( CONST FLOAT * v)		: D3DXMATRIX(v) {}
	Matrix( CONST D3DMATRIX& v)		: D3DXMATRIX(v) {}
	Matrix( CONST D3DXFLOAT16 * v)	: D3DXMATRIX(v) {}
    Matrix( FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
                FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
                FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
				FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44 ) :
			D3DXMATRIX(  _11,  _12,  _13,  _14,
						 _21,  _22,  _23,  _24,
						 _31,  _32,  _33,  _34,
						 _41,  _42,  _43,  _44 ) {}	
	// TODO
};

typedef Vect3 Vector3;

const Point3 ORIGIN(0,0,0); 
const Vect3 X(1,0,0); 
const Vect3 Y(0,1,0); 
const Vect3 Z(0,0,1);

const Matrix IDENTITY(1,0,0,0, 
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1);

#endif
