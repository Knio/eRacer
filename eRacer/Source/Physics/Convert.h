#ifndef CONVERT_H_
#define CONVERT_H_

#include "NxPhysics.h"
#include "Core/Math.h"

namespace Physics {

Vector3 NxVec3_Vector3(const NxVec3 &v);
NxVec3  Vector3_NxVec3(const Vector3 &v);
Matrix  NxMat33_Matrix(const NxMat33 &m);
NxMat33 Matrix_NxMat33(const Matrix &m);

}
#endif
