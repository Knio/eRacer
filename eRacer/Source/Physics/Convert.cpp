#include "Convert.h"

namespace Physics {

Vector3 NxVec3_Vector3(const NxVec3 &v)
{
  return Vector3(v.x, v.y, v.z);
}

NxVec3 Vector3_NxVec3(const Vector3 &v)
{
  return NxVec3(v.x, v.y, v.z);
}

Matrix NxMat33_Matrix(const NxMat33 &m)
{
  return Matrix(
    m(0, 0),  m(1, 0),  m(2, 0),  0,
    m(0, 1),  m(1, 1),  m(2, 1),  0,
    m(0, 2),  m(1, 2),  m(2, 2),  0,
    0,        0,        0,        1
  );
}
NxMat33 Matrix_NxMat33(const Matrix &m)
{ // UNTESTED! Might need a transpose
  return NxMat33(
    NxVec3(m._11, m._21, m._31),
    NxVec3(m._12, m._22, m._32),
    NxVec3(m._13, m._23, m._33)
  );  
}


}
