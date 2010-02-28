/**
 * @file BoundingSphere.h
 * @brief Definition of the BoundingSphere class
 *
 * @date 10.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Core/Types.h"
#include "Core/Math.h"

namespace Graphics {

/**
 * @brief A bounding volume that is a sphere.
 */
class BoundingSphere : public Sphere {
public:
	/**
	 * @brief recompute the sphere from a set of points.
	 *
	 * This method uses an algorithm that takes the average of all points as center and
	 * computes the minimally necessary radius to contain all points. The resulting sphere
	 * is generally not the minimal sphere containing all points, but a good approximation.
	 *
	 * @param vertices 
	 *		a pointer to the memory where the vertices are stored. The method assumes
	 *		that this pointer is pointing to the position of the first vertex
	 * @param nVertices
	 *		the number of vertices to be taken into account
	 * @param bytesPerVertex
	 *		the number of bytes used to store one vertex
	 */
	void recompute(const unsigned char* vertices, unsigned int nVertices, unsigned int bytesPerVertex);

	/**
	 * @brief set radius to 0 and center to (0,0,0)
	 */
	void reset();

	/**
	 * @brief expand this sphere such that it also contains the given sphere
	 *
	 * @param sphere
	 *			the sphere to merge with this sphere
	 */
	void merge(const BoundingSphere& sphere);

	/**
	 * @brief cull this sphere against the given plane
	 *
	 * @param plane 
	 *			the plane to cull against
	 * @returns true if the sphere is entirely on the negative side of the plane, else false
	 */
	bool cull(const Plane& plane) const; 
};


inline void BoundingSphere::reset(){
	center = ORIGIN;
	radius = 0.0f;
}

}