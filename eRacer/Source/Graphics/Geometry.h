/**
 * @file Geometry.h
 * @brief Definition of the Geometry class
 *
 * @date 22.01.2010
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
 * @brief base class for different kinds of geometry
 * 
 * @see MovingGeometry
 * @see StaticGeometry
 */
class Geometry : public Spatial
{
public:
	/**
	 * @brief Destructor stub. Virtual so that subclasse's constructors will be called
	 *
	 */
	virtual ~Geometry();

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
	virtual void cullRecursive(const Camera& camera, vector<const Geometry*>& visibleNodes) const;

	/**
	 * @brief getter for the mesh
	 *
	 * @return a const pointer to the mesh
	 */
	const LPD3DXMESH GetMesh() const;
	
	/**
	 * @brief setter for the mesh. Has to be implemented by subclasses
	 *
	 * It also updates the bounding volume(s)
	 *
	 * @param mesh
	 *			a pointer to the mesh for this geometry
	 */
	virtual void SetMesh(const LPD3DXMESH mesh) = 0;

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
	 * @brief Constructor. Only for inheriting classes because this class is abstract.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 */
	Geometry(const string& name);

	void UpdateModelBounds(AxisAlignedBoundingBox& modelBounds);
	void UpdateWorldBounds(const AxisAlignedBoundingBox& modelBounds,AxisAlignedBoundingBox& worldBounds);


	Matrix transform_;
	LPD3DXMESH mesh_;
	vector<D3DMATERIAL9*> materials_;
	vector<LPDIRECT3DTEXTURE9> textures_;
};

inline const LPD3DXMESH Geometry::GetMesh() const{
	return mesh_;
}


inline const vector<D3DMATERIAL9*>& Geometry::Materials() const{
	return materials_;
}

inline vector<D3DMATERIAL9*>& Geometry::Materials(){
	return materials_;
}

inline const vector<LPDIRECT3DTEXTURE9>& Geometry::Textures() const{
	return textures_;
}

inline vector<LPDIRECT3DTEXTURE9>& Geometry::Textures(){
	return textures_;
}

};
