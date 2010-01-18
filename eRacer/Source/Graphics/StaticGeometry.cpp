/**
 * @file StaticGeometry.cpp
 * @brief Implementation of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "StaticGeometry.h"

namespace Graphics {

StaticGeometry::StaticGeometry(const string& name, const Matrix& transform, TriMesh* geometry)
	: Spatial(name),
	  transform_(transform),
	  geometry_(geometry)
{
	assert(0 != geometry);
	//TODO compute the world bounding volume
}

StaticGeometry::~StaticGeometry(){
	geometry_.reset();
}

void StaticGeometry::cullRecursive(const Camera&, vector<const StaticGeometry*>& visibleNodes) const{
	visibleNodes.push_back(this);
}

StaticGeometry::StaticGeometry(const string& name)
	: Spatial(name),
	  transform_(IDENTITY)
{
	
}

};
