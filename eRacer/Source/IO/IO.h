#ifndef IO_H_
#define IO_H_

#include <string>
#include "..\Graphics\Geometry.h"

using namespace std;

class IO 
{
	static IO* g_IO;
  protected:
	IO(LPDIRECT3DDEVICE9 d) { g_IO = this; d3dd = d; }

  public:
	static IO* GetInstance(); 

	LPDIRECT3DDEVICE9 d3dd;  
	
	virtual ~IO() {}
	// TODO this should return a tuple (mesh, materials, textures)
	virtual int LoadMesh(Graphics::Geometry* geom, const char* file);
	virtual LPDIRECT3DTEXTURE9 LoadTexture(const char* file);
	static bool valid(LPDIRECT3DTEXTURE9 t) { return t != (LPDIRECT3DTEXTURE9)-1; }
};

inline IO* IO::GetInstance(){
	assert(g_IO);
	return g_IO;
}

#endif
