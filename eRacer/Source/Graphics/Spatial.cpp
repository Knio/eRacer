/**
 * @file Spatial.cpp
 * @brief Implementation of the Spatial class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "Spatial.h"

namespace Graphics {


Spatial::Spatial(const string& name)
	: name_(name),
	  visible(true)
{
	
}

Spatial::~Spatial(){
}

void Spatial::cull(const Camera& camera, vector<const Renderable*>& visibleRenderables) const{
	if(!visible)
		return;

	for(int i=0; i<6; i++){
		if(worldBounds_.cull(camera.GetPlane(i)))
			return;
	}

	cullRecursive(camera, visibleRenderables);
}


};
