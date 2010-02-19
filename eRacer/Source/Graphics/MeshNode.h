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
 * @brief Abstract base class for meshes that are part of the scene graph
 * 
 * @see Mesh
 * @see MovingMeshNode
 * @see StaticMeshNode
 */
class MeshNode : public Spatial, public Mesh
{
public:
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
	 * @brief Add myself to the list
	 *
	 * @param camera
	 *			The camera to cull against - not needed here
	 * @param visibleNodes
	 * 			A vector to push this node to
	 *
	 * @see Spatial::cullRecursive
	 */
	virtual void cullRecursive(const Camera&, vector<const MeshNode*>& visibleNodes) const;

	/**
	 * @brief Getter for the world space transform
	 * @return the world space transform of this node
	 */
	const Matrix& GetTransform() const;

	/**
	 * @brief Setter for the mesh
	 * 
	 * Also updates the bounding volume. This setter can only be called once.
	 *
	 * @param mesh 
	 *			the Direct3D mesh to set
	 */
	virtual void SetMesh(ID3DXMesh* mesh);

protected:
	/**
	 * @brief Constructor. Only for inheriting classes because this class is abstract.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 */
	MeshNode(const string& name);

	/**
	 * @brief update the bounding volume from the mesh data
	 *
	 * This method should be called whenever vertex data changes (i.e. after loading)
	 * to bring the bounding volume up to date.
	 */
	void UpdateBounds();

	/**
	 * @brief the world transformation matrix to apply to all vertices to allow for instancing
	 */
	Matrix transform_;

};

inline const Matrix& MeshNode::GetTransform() const { 
	return transform_; 
}



}
