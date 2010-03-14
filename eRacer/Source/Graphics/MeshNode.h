/**
 * @file MeshNode.h
 * @brief Definition of the MeshNode class
 *
 * @date 22.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "RenderableNode.h"
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
class MeshNode : public RenderableNode
{
public:
	/**
	 * @brief Constructor. 
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
	virtual void UpdateWorldBounds();


	Mesh* mesh_;
};

inline void MeshNode::UpdateWorldBounds(){
	if(!initialized)	
		return;
		
	worldBounds_.center = mul0(transform_, mesh_->localBounds.center);
	float x, y, z;
	ExtractScaling(transform_,x,y,z);
	worldBounds_.radius = mesh_->localBounds.radius * max(max(x,y),z);
}




}
