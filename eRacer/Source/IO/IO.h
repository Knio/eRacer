#ifndef IO_H_
#define IO_H_

#include "..\Graphics\Geometry.h"

class IO 
{
	IO* g_IO;
	LPDIRECT3DDEVICE9 d3dd;
  public:
	IO(LPDIRECT3DDEVICE9 d) { g_IO = this; d3dd = d; }
	virtual ~IO() {}
	// TODO this should return a tuple (mesh, materials, textures)
	virtual int LoadMesh(Graphics::Geometry* geom, const char* file);
	virtual LPDIRECT3DTEXTURE9 LoadTexture(const char* file);
	static bool valid(LPDIRECT3DTEXTURE9 t) { return t != (LPDIRECT3DTEXTURE9)-1; }
};


#endif

