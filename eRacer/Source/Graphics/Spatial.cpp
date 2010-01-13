/**
 * @file Spatial.cpp
 * @brief Implementation of the Spatial class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "Spatial.h"

Spatial::Spatial(const string& name)
	: name_(name),
	  visible_(true)
{
	
}

Spatial::~Spatial(){
}

void Spatial::onDraw(GraphicsLayer& renderer) const{
	if(!visible_)
		return;

	//TODO: culling

	draw(renderer);
}

