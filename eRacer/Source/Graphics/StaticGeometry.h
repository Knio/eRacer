/**
 * @file StaticGeometry.h
 * @brief Definition of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Geometry.h"
#include <vector>
#include "d3d9types.h"
#include "d3dx9mesh.h"

using namespace std;

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
class StaticGeometry : public Geometry
{
public:
	/**
	 * @brief Constructor. Set transformation and geoemtry.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 * @param transform
	 *			the transformation to apply to the geometry (for instancing)
	 */
	StaticGeometry(const string& name, const Matrix& transform);

	/**
	 * @brief Destructor stub.
	 */
	virtual ~StaticGeometry();

	/**
	 * @brief setter for the mesh
	 *
	 * This setter can only be called once. It also updates the bounding volume
	 *
	 * @param mesh
	 *			a pointer to the mesh for this geometry
	 */
	virtual void SetMesh(const LPD3DXMESH mesh);


protected:
};

};
