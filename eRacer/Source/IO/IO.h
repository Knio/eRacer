#ifndef IO_H_
#define IO_H_

#include <string>
#include "..\Graphics\Geometry.h"

using namespace std;

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
	virtual int LoadMesh(Graphics::Geometry* geom, const char* file)	{ assert(false); return 0; }

	/** Load a texture. returns (LPDIRECT3DTEXTURE9)-1 on failure. NULL is a valid, empty texture */
	virtual LPDIRECT3DTEXTURE9 LoadTexture(const char* file)			{ assert(false); return NULL; }
	
	/** Check if a texture is valid */
	static bool valid(LPDIRECT3DTEXTURE9 t) { return t != (LPDIRECT3DTEXTURE9)-1; }


	int _LoadMesh(Graphics::Geometry* geom, const char* file);
	LPDIRECT3DTEXTURE9 _LoadTexture(const char* file);	

};

inline IO* IO::GetInstance(){
	assert(g_IO);
	return g_IO;
}

#endif
