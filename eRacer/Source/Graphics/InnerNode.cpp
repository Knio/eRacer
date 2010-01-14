/**
 * @file .cpp
 * @brief Implementation of the  class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "InnerNode.h"

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

void InnerNode::draw(GraphicsLayer& renderer) const{
	for(vector<Spatial*>::const_iterator i = children_.begin();
		i != children_.end(); i++){
		(*i)->onDraw(renderer);
	}
}

void InnerNode::addChild(Spatial* spatial){
	children_.push_back(spatial);
	worldBoundingVolume_.merge(spatial->getWorldBoundingVolume());
}

void InnerNode::removeChild(Spatial* spatial){
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
