/**
 * @file Renderable.h
 * @brief Definition of the Renderable class
 *
 * @date 05.02.2010
 * @author: Tom Flanagan
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "D3D9.h."

class Renderable
{
public:
	virtual void Draw(IDirect3DDevice9*) const = 0;
};

#endif
