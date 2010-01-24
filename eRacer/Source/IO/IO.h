#ifndef IO_H_
#define IO_H_

#include <string>
#include "..\Graphics\Geometry.h"
#include "..\Graphics\GraphicsLayer.h"

using namespace std;

class IO 
{
	static IO* g_IO;

	LPDIRECT3DDEVICE9 d3dd;

	IO();
  public:
	static const string TEXTURE_FOLDER;

	static IO* GetInstance(); 

	IO(LPDIRECT3DDEVICE9 d) { g_IO = this; d3dd = d; }
	virtual ~IO() {}
	// TODO this should return a tuple (mesh, materials, textures)
	virtual int LoadMesh(Graphics::Geometry* geom, const char* file);
	virtual LPDIRECT3DTEXTURE9 LoadTexture(const char* file);
	static bool valid(LPDIRECT3DTEXTURE9 t) { return t != (LPDIRECT3DTEXTURE9)-1; }
};

inline IO* IO::GetInstance(){
	if(NULL == g_IO)
		g_IO = new IO();
	return g_IO;
}

inline IO::IO(){
	d3dd = Graphics::GraphicsLayer::GetGraphicsInstance()->GetDevice();
}


#endif
