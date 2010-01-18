/**
 * @file StaticGeometry.h
 * @brief Definition of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Spatial.h"
#include "boost/smart_ptr.hpp" 
#include "TriMesh.h"

namespace Graphics {

/**
 * @brief node containing non-moving geometry
 *
 * A non-moving geometry node can, once created, not be transformed or 
 * be associated with different geometry. This ensures that the bounding
 * volumes do not change over time.
 * 
 * @see MovingGeometryNode
 */
class StaticGeometry : public Spatial
{
public:
	/**
	 * @brief Constructor. Set transformation and geoemtry.
	 *
	 * This will also compute the world bounding volume.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 * @param transform
	 *			the transformation to apply to the geometry (for instancing)
	 * @param geometry
	 *			a const pointer to the geometry - geometry can be shared 
	 *			among many nodes. must not be NULL
	 */
	StaticGeometry(const string& name, const Matrix& transform, TriMesh* geometry);
	virtual ~StaticGeometry();

	/**
	 * @brief Add myself to the list
	 *
	 * @param camera
	 *			The camera to cull against - not needed here
	 * @param visibleNodes
	 * 			A vector to push this node to
	 *
	 * @see Spatial::cullRecursive
	 */
	virtual void cullRecursive(const Camera& camera, vector<const StaticGeometry*>& visibleNodes) const;

protected:
	/**
	 * @brief Constructor. Only for inheriting classes.
	 *
	 * This will also compute the world bounding volume.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 */
	StaticGeometry(const string& name);


	Matrix transform_;
	boost::shared_ptr<TriMesh> geometry_;
};

};
