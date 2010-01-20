/**
 * @file StaticGeometry.h
 * @brief Definition of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Spatial.h"
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
	 */
	StaticGeometry(const string& name, const Matrix& transform);
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

	/**
	 * @brief getter for the mesh
	 *
	 * @return a const pointer to the mesh
	 */
	const LPD3DXMESH GetMesh() const;
	
	/**
	 * @brief setter for the mesh
	 *
	 * Since this is the class for static geometry, this setter can only be called once.
	 *
	 * @param mesh
	 *			a pointer to the mesh for this geometry
	 */
	void SetMesh(const LPD3DXMESH mesh);

	/**
	 * @brief read access to the vector of materials
	 *
	 * @return read-only referece to the vector of materials
	 */
	const vector<D3DMATERIAL9*>& Materials() const;
	
	/**
	 * @brief read/write access to the vector of materials
	 * 
	 * @return read/write reference to the vector of materials
	 */
	vector<D3DMATERIAL9*>& Materials();

	/**
	 * @brief read access to the vector of textures
	 *
	 * @return read-only referece to the vector of textures
	 */
	const vector<LPDIRECT3DTEXTURE9>& Textures() const;

	/**
	 * @brief read/write access to the vector of textures
	 * 
	 * @return read/write reference to the vector of textures
	 */
	vector<LPDIRECT3DTEXTURE9>& Textures();

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
	LPD3DXMESH mesh_;
	vector<D3DMATERIAL9*> materials_;
	vector<LPDIRECT3DTEXTURE9> textures_;
};

inline const LPD3DXMESH StaticGeometry::GetMesh() const{
	return mesh_;
}


inline const vector<D3DMATERIAL9*>& StaticGeometry::Materials() const{
	return materials_;
}

inline vector<D3DMATERIAL9*>& StaticGeometry::Materials(){
	return materials_;
}

inline const vector<LPDIRECT3DTEXTURE9>& StaticGeometry::Textures() const{
	return textures_;
}

inline vector<LPDIRECT3DTEXTURE9>& StaticGeometry::Textures(){
	return textures_;
}

};
