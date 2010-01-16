
#ifndef TYPES_H_
#define TYPES_H_

#include <d3dx9math.h>

#define PI D3DX_PI

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR3 Point3;
typedef D3DXMATRIX Matrix;

const Point3 ORIGIN(0,0,0); 
const Vector3 X(1,0,0); 
const Vector3 Y(0,1,0); 
const Vector3 Z(0,0,1);

const Matrix IDENTITY(1,0,0,0, 
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1);


#include "vect3.h"



struct Plane {
	Vector3 normal;
	float distance;
};

#endif // TYPES_H_
