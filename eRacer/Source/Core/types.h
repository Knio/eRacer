
#ifndef TYPES_H_
#define TYPES_H_

#include <d3dx9math.h>

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR3 Point3;
typedef D3DXMATRIX Matrix;

const Point3 ORIGIN(0,0,0); 
const Matrix IDENTITY(1,0,0,0, 
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1);

#endif // TYPES_H_