/**
 * @file MeshNode.h
 * @brief Definition of the MeshNode class
 *
 * @date 22.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Spatial.h"
#include "Mesh.h"

#include "d3d9types.h"
#include "d3dx9mesh.h"

#include <vector>

using namespace std;

namespace Graphics {

/**
 * @brief Node that contains a Mesh and is part of the scene graph
 * 
 * @see Mesh
 */
class MeshNode : public Spatial, public Renderable
{
public:
	/**
	 * @brief Constructor. Only for inheriting classes because this class is abstract.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 */
	MeshNode(const string& name, const Matrix& tx=IDENTITY);

	/**
	 * @brief Destructor stub. Virtual so that sub class' destructors will be called
	 *
	 */
	virtual ~MeshNode();

	/**
	 * @brief Draw the mesh associated with this mesh node.
	 * 
	 * In addition to mesh drawing, this method uses the world transform
	 * matrix to position the mesh at the correct position
	 *
	 * @param device 
	 *			the direct 3d device to use for drawing
	 */
	virtual void Draw(IDirect3DDevice9* device) const;

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

	/**
	 * @brief initialize this mesh. Can be overidden by subclasses
	 *
	 * @param mesh
	 *			a pointer to a mesh wrapper
	 */
	virtual void Init(Mesh* mesh);

	bool initialized;
protected:

	/**
	 * @brief update the world bounding volume by transforming the local bounding volume
	 *
	 * This method should be called whenever the transform of the mesh node changes.
	 */
	void UpdateWorldBounds();

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

	Mesh* mesh_;


};

inline const Matrix& MeshNode::GetTransform() const { 
	return transform_; 
}

inline void MeshNode::UpdateWorldBounds(){
	worldBounds_.center = mul0(transform_, mesh_->localBounds.center);
	float x, y, z;
	ExtractScaling(transform_,x,y,z);
	worldBounds_.radius = mesh_->localBounds.radius * max(max(x,y),z);
}




}
