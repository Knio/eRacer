/**
 * @file MovingMeshNode.h
 * @brief Definition of the MovingMeshNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "MeshNode.h"

namespace Graphics {


/**
 * @brief a node containing moving geometry
 *
 * Moving geometry is geometry that may change its transform over time. 
 * This requires updating of bounding boxes.
 * Moving geometry may not change its mesh over time! Another subclass should be dedicated 
 * to this if necessary.
 */
class MovingMeshNode : public MeshNode
{
public:
	/**
	 * @brief Constructor.
	 */
	MovingMeshNode(const string& name, const Matrix& tx=IDENTITY);
	
	/**
	 * @brief Destructor stub.
	 */
	virtual ~MovingMeshNode();

	/**
	 * @brief setter for the world transform of this node
	 *
	 * This method will also update world bounding box
	 *
	 * @param transform
	 *			the new transform from model space to world space for this node
	 */
	void SetTransform(const Matrix& transform);

};


};
