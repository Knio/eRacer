/**
 * @file QuadNode.h
 * @brief Definition of the QuadNode class
 *
 * @date 21.02.2010
 * @author: Ole Rehmsen
 */
#pragma once

#include "RenderableNode.h"

#include "Core/Math.h"

namespace Graphics{

/**
 * @brief a textured quad with a location, rotation and scaling in space
 */
class QuadNode : public RenderableNode {
	struct Vertex {
		Point3 position;
		//DWORD color;
	    float u;
	    float v;

	};

public:

	/**
	 * @brief constructor
	 */
	QuadNode(const string& name, const Matrix& transform = IDENTITY);

	/**
	 * @brief destructor stub
	 */
	virtual ~QuadNode() {};

	/**
	 * @brief initialize this quad. Can be overidden by subclasses
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
	 * @brief set the texture coordinates to custom values
	 */
	void SetTextureCoordinates(	float tl_u=0.0f, float tl_v=0.0f, 
								float tr_u=1.0f, float tr_v=0.0f, 
								float br_u=1.0f, float br_v=1.0f, 
								float bl_u=0.0f, float bl_v=1.0f);


	/**
	 * @brief flag initially set to false indicating whether the texture has been loaded already
	 */
	bool initialized;

protected:
	/**
	 * @brief update the world bounding volume by transforming the local bounding volume
	 *
	 * This method should be called whenever the transform of the mesh node changes.
	 */
	virtual void UpdateWorldBounds();

	IDirect3DTexture9* texture_;
	IDirect3DVertexBuffer9* vertexBuffer_;
};


}