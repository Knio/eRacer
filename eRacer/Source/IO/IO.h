#ifndef IO_H_
#define IO_H_

#include <cassert>
#include <iostream>
#include <vector>

#define NOMINMAX
#include <windows.h>
#include <d3d9types.h>
#include <d3dx9mesh.h>

#include "Graphics\Mesh.h"
#include "Graphics\BoundingSphere.h"

using namespace std;


struct CachedMesh{
	ID3DXMesh* d3dMesh;
	unsigned int nMaterials;
	D3DMATERIAL9* materials;
	vector<string> texturePatterns;
	Graphics::BoundingSphere localBounds;	
	
	bool IsValid() const;
};


class IO 
{

protected:
	static IO* g_IO;
	// TODO don't stogitre this pointer
	IDirect3DDevice9* d3dd;
	IO(IDirect3DDevice9* d) { g_IO = this; d3dd = d; }

	
public:
	static IO* GetInstance(); 
 
 	virtual ~IO() {}
 	// These methods implemented in IO/__init__.py
	// TODO this should return a tuple (mesh, materials, textures)

	/** Load a .x mesh into a Geometry node. This will also load any textures defined in the mesh */
	virtual int LoadMesh(Graphics::Mesh* model, const char* file)		{ assert(false); return 0; }

	/** Load a texture. returns (LPDIRECT3DTEXTURE9)-1 on failure. NULL is a valid, empty texture */
	virtual IDirect3DTexture9* LoadTexture(const char* file)			{ assert(false); return NULL; }
	
	/** Check if a texture is valid */
	static bool valid(IDirect3DTexture9* t) { return t != (IDirect3DTexture9*)-1; }

	// private
	bool _LoadMesh(const char* file, CachedMesh& mesh);
	IDirect3DTexture9* _LoadTexture(const char* file);	
	void _FreeTexture(IDirect3DTexture9* t);
};

inline IO* IO::GetInstance(){
	assert(g_IO);
	return g_IO;
}

#endif
