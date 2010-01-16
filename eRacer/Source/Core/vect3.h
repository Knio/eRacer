
#ifndef VECT3_H_
#define VECT3_H_

#include "types.h"

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


#endif
