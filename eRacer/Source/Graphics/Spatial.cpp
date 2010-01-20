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
	  visible_(true)
{
	
}

Spatial::~Spatial(){
}

void Spatial::cull(const Camera& camera, vector<const StaticGeometry*>& visibleNodes) const{
	if(!visible_)
		return;

	for(int i=0; i<6; i++){
		const Plane& plane = camera.getPlane(i);

		Point3 boxCenter = 0.5*(worldBoundingVolume_.getMin()+worldBoundingVolume_.getMax());
		Vector3 toMax = worldBoundingVolume_.getMax()-boxCenter;

		//project all vertices of the box on the plane
		//and compute radius of the interval around the center of the box
		float radius = toMax.x * abs(dot(plane.normal,X))
						+ toMax.y * abs(dot(plane.normal,Y))
						+ toMax.z * abs(dot(plane.normal,Z));

		if(dot(plane.normal, boxCenter)-plane.distance < -radius)
			return;
	}

	cullRecursive(camera, visibleNodes);
}


};
