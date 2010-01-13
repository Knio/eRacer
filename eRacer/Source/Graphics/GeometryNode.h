/**
 * @file GeometryNode.h
 * @brief Definition of the GeometryNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Spatial.h"
#include "boost/smart_ptr.hpp" 
#include "TriMesh.h"

/**
 * @brief node containing non-moving geometry
 *
 * A non-moving geometry node can, once created, not be transformed or 
 * be associated with different geometry. This ensures that the bounding
 * volumes do not change over time.
 * 
 * @see MovingGeometryNode
 */
class GeometryNode : public Spatial
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
	 *			among many nodes
	 */
	GeometryNode(const string& name, const Matrix& transform, TriMesh* geometry);
	virtual ~GeometryNode();

	/**
	 * @brief prepare for and trigger rendering
	 *
	 * @param renderer
	 *			the renderer to use for rendering
	 *
	 * @see Spatial::draw
	 */
	virtual void draw(GraphicsLayer& renderer) const;

protected:
	boost::shared_ptr<TriMesh> geometry_;
	Matrix transform_;
};

