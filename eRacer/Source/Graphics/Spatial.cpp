/**
 * @file Spatial.cpp
 * @brief Implementation of the Spatial class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "Spatial.h"
#include <iostream>

using namespace std;

namespace Graphics {


Spatial::Spatial(const string& name)
	: name_(name),
	  visible(true) // does this still do anything??
{
	
}

Spatial::~Spatial(){
}

void Spatial::cull(const Camera& camera, vector<const Renderable*>& visibleRenderables) const{
	
	if(!visible)
		return;
	
	for(int i=0; i<PI_NUM; i++){
		if(worldBounds_.cull(camera.GetPlane(i))){
			cout << name_ << " culled by plane " << i << "!"<<endl;	 	
			return;
		}
	}
	

	cullRecursive(camera, visibleRenderables);
}


};
