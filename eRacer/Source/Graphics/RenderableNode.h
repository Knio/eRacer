/**
 * @file RenderableNode.h
 * @brief Definition of the RenderableNode class
 *
 * @date 22.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Spatial.h"

#include "d3d9types.h"

#include <vector>

using namespace std;

namespace Graphics {

/**
 * @brief Abstract base class for nodes of the scene graph that can be rendered
 */
class RenderableNode : public Spatial, public Renderable
{
public:
	/**
	 * @brief Constructor. 
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 */
	RenderableNode(const string& name, const Matrix& tx=IDENTITY);

	/**
	 * @brief Destructor stub. Virtual so that sub class' destructors will be called
	 *
	 */
	virtual ~RenderableNode();

	/**
	 * @brief Getter for the world space transform
	 * @return the world space transform of this node
	 */
	const Matrix& GetTransform() const;

	/**
	 * @brief setter for the world transform of this node
	 *
	 * This method will also update world bounding box
	 *
	 * @param transform
	 *			the new transform from model space to world space for this node
	 */
	void SetTransform(const Matrix& transform);

protected:

	/**
	 * @brief update the world bounding volume by transforming the local bounding volume
	 *
	 * This method should be called whenever the transform of the mesh node changes.
	 */
	virtual void UpdateWorldBounds() = 0;

	/**
	 * @brief Add myself to the list
	 *
	 * @param visibleRenderables
	 * 			A vector to push this node to
	 *
	 * @see Spatial::cullRecursive
	 */
	virtual void cullRecursive(const Camera&, vector<const Renderable*>& visibleRenderables) const;


	/**
	 * @brief the world transformation matrix to apply to all vertices to allow for instancing
	 */
	Matrix transform_;
};

inline const Matrix& RenderableNode::GetTransform() const { 
	return transform_; 
}




}
