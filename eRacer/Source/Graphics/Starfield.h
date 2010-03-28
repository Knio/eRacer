#ifndef STARFIELD_H_
#define STARFIELD_H_

#include "assert.h"
#include "Core/Types.h"
#include "Renderable.h"
#include "Camera.h"

#define MAX_LEN (32)

namespace Graphics {

struct Star {
	Point3 pos;
	DWORD color;
};

class Starfield : public Renderable
{
	float SIZE;
	int 	N;
	
	Matrix* view;
	int curview;
	Point3 	pos;
	
	
	Star* stars;
	LPDIRECT3DVERTEXBUFFER9 vb;
	
	ID3DXEffect* effect;
	
public:
	int length;
	Starfield(int n, float s);
	void Update(const Matrix& newview, const Point3 &newpos);
	virtual void Draw(IDirect3DDevice9*) const;
};

}

#endif