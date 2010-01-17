/**
 * @file GeometryNode.cpp
 * @brief Implementation of the GeometryNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "GeometryNode.h"

namespace Graphics {

GeometryNode::GeometryNode(const string& name, const Matrix& transform, TriMesh* geometry)
	: Spatial(name),
	  transform_(transform),
	  geometry_(geometry)
{
	assert(0 != geometry);
	//TODO compute the world bounding volume
}

GeometryNode::~GeometryNode(){
	geometry_.reset();
}

void GeometryNode::cullRecursive(const Camera&, vector<const GeometryNode*>& visibleNodes) const{
	visibleNodes.push_back(this);
}

GeometryNode::GeometryNode(const string& name)
	: Spatial(name),
	  transform_(IDENTITY)
{
	
}

}
