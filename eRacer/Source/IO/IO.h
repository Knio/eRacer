#ifndef IO_H_
#define IO_H_

#pragma message("IOH1")

#include <windows.h>
#include <cassert>
#include <d3d9types.h>
#include <d3dx9mesh.h>

#pragma message("IOH2")
#include "Graphics\Model.h"
#pragma message("IOH3")


struct Mesh
{
	DWORD				n;
	LPD3DXMESH			mesh;
	D3DMATERIAL9*		materials;
	PDIRECT3DTEXTURE9*	textures;
};

class IO 
{

protected:
	static IO* g_IO;
	LPDIRECT3DDEVICE9 d3dd;
	IO(LPDIRECT3DDEVICE9 d) { g_IO = this; d3dd = d; }

	
public:
	static IO* GetInstance(); 
 
 	virtual ~IO() {}
 	// These methods implemented in IO/__init__.py
	// TODO this should return a tuple (mesh, materials, textures)

	/** Load a .x mesh into a Geometry node. This will also load any textures defined in the mesh */
	virtual int LoadMesh(Graphics::Model* model, const char* file)		{ assert(false); return 0; }

	/** Load a texture. returns (LPDIRECT3DTEXTURE9)-1 on failure. NULL is a valid, empty texture */
	virtual LPDIRECT3DTEXTURE9 LoadTexture(const char* file)			{ assert(false); return NULL; }
	
	/** Check if a texture is valid */
	static bool valid(LPDIRECT3DTEXTURE9 t) { return t != (LPDIRECT3DTEXTURE9)-1; }
	/** Check if a mesh is valid */
	static bool valid(Mesh &m) { return m.n != -1; }

	// private
	Mesh _LoadMesh(const char* file);
	void _SetMesh(Graphics::Model* model, Mesh &Mesh);
	LPDIRECT3DTEXTURE9 _LoadTexture(const char* file);	
	void _FreeTexture(LPDIRECT3DTEXTURE9 t);
	void _FreeMesh(Mesh &m);
};

inline IO* IO::GetInstance(){
	assert(g_IO);
	return g_IO;
}

#endif
#pragma message("IOH4")
