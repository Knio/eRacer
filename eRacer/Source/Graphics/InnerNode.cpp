/**
 * @file InnerNode.cpp
 * @brief Implementation of the InnerNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "InnerNode.h"

namespace Graphics {

InnerNode::InnerNode(const string& name)
	: Spatial(name)
{

}

InnerNode::~InnerNode(){
	for(vector<Spatial*>::iterator i = children_.begin();
		i != children_.end(); i++){
		delete *i;
		*i=0;
	}
}

void InnerNode::cullRecursive(const Camera& camera, vector<const Renderable*>& visibleRenderables) const{
	for(vector<Spatial*>::const_iterator i = children_.begin();
		i != children_.end(); i++){
		(*i)->cull(camera,visibleRenderables);
	}
}

void InnerNode::addChild(Spatial* spatial){
	children_.push_back(spatial);
	worldBounds_.merge(spatial->getWorldBounds());
}

void InnerNode::removeChild(Spatial* spatial){
	assert(false);
	/*
	worldBoundingVolume_.reset();
	for(vector<Spatial*>::iterator i = children_.begin();
		i != children_.end(); i++){
		if(*i == spatial)
			children_.erase(i);
		else
			worldBoundingVolume_.merge((*i)->getWorldBoundingVolume());
	}
	*/
	//TODO destroy child?
}

};
