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



}
