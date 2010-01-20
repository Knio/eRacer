/**
 * @file StaticGeometry.cpp
 * @brief Implementation of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "StaticGeometry.h"

namespace Graphics {

StaticGeometry::StaticGeometry(const string& name, const Matrix& transform)
	: Spatial(name),
	  transform_(transform)
{
	//TODO compute the world bounding volume
}

StaticGeometry::~StaticGeometry(){
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
