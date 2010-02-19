/**
 * @file StaticMeshNode.h
 * @brief Definition of the StaticMeshNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "MeshNode.h"
#include <vector>

using namespace std;

namespace Graphics {

/**
 * @brief node containing non-moving geometry
 *
 * A non-moving geometry node can, once created, not be transformed or 
 * be associated with different geometry. This ensures that the bounding
 * volumes do not change over time.
 * 
 * @see MovingMeshNode
 */
class StaticMeshNode : public MeshNode
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
	StaticMeshNode(const string& name, const Matrix& transform);

	/**
	 * @brief Destructor stub.
	 */
	virtual ~StaticMeshNode();

};

}
