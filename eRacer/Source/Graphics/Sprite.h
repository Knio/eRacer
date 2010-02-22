/**
 * @file Sprite.h
 * @brief Definition of the Sprite class
 *
 * @date 21.02.2010
 * @author: Ole Rehmsen
 */
#pragma once

#include "Renderable.h"

#include "Core/Math.h"

namespace Graphics{

/**
 * @brief a textured quad with a location, rotation and scaling in space
 */
class Sprite : public Renderable {
	struct Vertex {
		Point3 position;
		DWORD color;
	    float u;
	    float v;

	};

public:

	/**
	 * @brief Constructor
	 */
	Sprite();

	/**
	 * @brief destructor stub
	 */
	virtual ~Sprite() {};

	/**
	 * @brief initialize this sprite. Can be overidden by subclasses
	 *
	 * @param texture
	 *			a pointer to the texture to put on the sprite
	 */
	virtual void Init(IDirect3DTexture9* texture);
	
	/**
	 * @brief draw a textured quad 
	 *
	 * @param device
	 *			the device to draw to
	 */
	virtual void Draw(IDirect3DDevice9* device) const;

	/**
	 * @brief setter for the world transform of this sprite
	 *
	 * @param transform
	 *			the new transform from model space to world space for this sprite
	 */
	void SetTransform(const Matrix& transform);

	/**
	 * @brief flag initially set to false indicating whether the texture has been loaded already
	 */
	bool initialized;

protected:
	Matrix transform_;
	IDirect3DTexture9* texture_;
	IDirect3DVertexBuffer9* vertexBuffer_;
};

inline void Sprite::SetTransform(const Matrix& transform){
	transform_ = transform;
}

}