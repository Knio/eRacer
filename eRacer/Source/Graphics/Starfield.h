#ifndef STARFIELD_H_
#define STARFIELD_H_

#include "assert.h"
#include "Core/Types.h"
#include "Renderable.h"

namespace Graphics {

struct Star {
	Point3 pos;
	DWORD color;
};

class Starfield : public Renderable
{
public:
	float SIZE;
	int N;
	Matrix oldcamera;
	Star* stars;
	LPDIRECT3DVERTEXBUFFER9 vb;
	Starfield(int n, float s);
	void Update(const Matrix &newcamera, const Point3 &pos);
	virtual void Draw(LPDIRECT3DDEVICE9) const;
};

}

#endif
