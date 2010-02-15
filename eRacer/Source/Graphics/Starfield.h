#ifndef STARFIELD_H_
#define STARFIELD_H_

#include "assert.h"
#include "Core/Types.h"
#include "Renderable.h"
#include "Camera.h"


namespace Graphics {

struct Star {
	Point3 pos;
	DWORD color;
};

class Starfield : public Renderable
{
	float SIZE;
	int 	N;
	
	Camera* camera;
	Matrix* view;
	Point3 	pos;
	
	Star* stars;
	LPDIRECT3DVERTEXBUFFER9 vb;
	
	void _Update();
	
public:
	Starfield(int n, float s);
	void Update();
	virtual void Draw(IDirect3DDevice9*) const;
};

}

#endif