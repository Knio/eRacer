/**
 * @file Renderable.h
 * @brief Definition of the Renderable class
 *
 * @date 05.02.2010
 * @author: Tom Flanagan
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "D3D9.h"

/**
 * @brief Abstract base class for all renderable objects
 *
 * A renderable is able to draw itself to a Direct3D device.
 */
class Renderable
{
public:
	/**
	 * @brief Virtual destructor stub so sub class destructors are called
	 */
	virtual ~Renderable() {}

	/**
	 * @brief Draw this renderable. To be implemented by sub classes.
	 *
	 * @param device
	 *			the device to draw to
	 */
	virtual void Draw(IDirect3DDevice9* device) const = 0;
};

#endif
