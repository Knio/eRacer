/**
 * @file SkyBox.h
 * @brief Definition of the SkyBox class
 *
 * @date 07.02.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Mesh.h"
#include "Camera.h"


namespace Graphics {

/**
 * @brief A skybox that always centers around a camera
 */
class SkyBox : public Renderable {
public:	
	SkyBox();

	void Init(Mesh* mesh);

	/**
	 * @brief draw the sky box
	 *
	 * @param device 
	 *			the device to draw to
	 */
	virtual void Draw(IDirect3DDevice9* device) const;

	bool initialized;
private:
	Mesh* mesh_;
};

}