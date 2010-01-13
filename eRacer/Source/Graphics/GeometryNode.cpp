/**
 * @file GeometryNode.cpp
 * @brief Implementation of the GeometryNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "GeometryNode.h"

GeometryNode::GeometryNode(const string& name, const Matrix& transform, TriMesh* geometry)
	: Spatial(name),
	  transform_(transform),
	  geometry_(geometry)
{
	assert(0 != geometry);
}

GeometryNode::~GeometryNode(){
	geometry_.reset();
}

void GeometryNode::draw(GraphicsLayer& renderer) const{
	renderer.draw(*geometry_);
}

GeometryNode::GeometryNode(const string& name)
	: Spatial(name),
	  transform_(IDENTITY)
{
	
}
