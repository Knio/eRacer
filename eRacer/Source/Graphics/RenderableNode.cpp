/**
 * @file RenderableNode.cpp
 * @brief Implementation of the RenderableNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "RenderableNode.h"
#include "GraphicsLayer.h"
#include "d3d9types.h"
#include <iostream>


using namespace std;

namespace Graphics {

RenderableNode::RenderableNode(const string& name, const Matrix& tx)
	: Spatial(name),
	  transform_(tx)
{
	
}


RenderableNode::~RenderableNode(){
}

void RenderableNode::cullRecursive(const Camera&, vector<const Renderable*>& visibleRenderables) const{
	visibleRenderables.push_back(this);
}

void RenderableNode::SetTransform(const  Matrix& transform){
	transform_ = transform;
	UpdateWorldBounds();
}


}
