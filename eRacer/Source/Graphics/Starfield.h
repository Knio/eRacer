#ifndef STARFIELD_H_
#define STARFIELD_H_

#include "assert.h"
#include "Core/Types.h"

struct Star {
	Point3 pos;
	DWORD color;
};

class Starfield
{
public:
	static const float SIZE;
	int N;
	Matrix camera;
	Star* stars;
	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DDEVICE9 dev;
	Starfield(LPDIRECT3DDEVICE9 d, int N=4096);
	void Update(const Matrix &c1, const Matrix &c2, const Point3 &pos);
	void Draw(const Matrix &newcamera, const Point3 &pos);


};
#endif
