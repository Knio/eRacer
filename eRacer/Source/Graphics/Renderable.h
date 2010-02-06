#ifndef RENDERABLE_H_
#define RENDERABLE_H_

class Renderable
{
public:
	Renderable() {};
	virtual void Draw(LPDIRECT3DDEVICE9) = 0;
};

#endif
