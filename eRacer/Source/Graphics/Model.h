

#pragma once

#include "d3d9types.h"
#include "d3dx9mesh.h"


namespace Graphics{

class Model {
public:
	Model();

	/**
	 * @brief getter for the mesh
	 *
	 * @return a const pointer to the mesh
	 */
	const ID3DXMesh* GetMesh() const;

	//TODO comment or remove
	//ID3DXMesh& mesh() { return *mesh_; };
	
	/**
	 * @brief setter for the mesh. Has to be implemented by subclasses
	 *
	 * It also updates the bounding volume(s)
	 *
	 * @param mesh
	 *			a pointer to the mesh for this geometry
	 */
	virtual void SetMesh(ID3DXMesh* mesh) = 0;

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
	const vector<IDirect3DTexture9*>& Textures() const;

	/**
	 * @brief read/write access to the vector of textures
	 * 
	 * @return read/write reference to the vector of textures
	 */
	vector<IDirect3DTexture9*>& Textures();

protected:
	ID3DXMesh* mesh_;
	vector<D3DMATERIAL9*> materials_;
	vector<IDirect3DTexture9*> textures_;
};

inline Model::Model()
: mesh_(NULL)
{
}

inline const ID3DXMesh* Model::GetMesh() const{
	return mesh_;
}


inline const vector<D3DMATERIAL9*>& Model::Materials() const{
	return materials_;
}

inline vector<D3DMATERIAL9*>& Model::Materials(){
	return materials_;
}

inline const vector<IDirect3DTexture9*>& Model::Textures() const{
	return textures_;
}

inline vector<IDirect3DTexture9*>& Model::Textures(){
	return textures_;
}

}