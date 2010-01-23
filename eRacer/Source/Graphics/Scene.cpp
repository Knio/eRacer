/**
 * @file Scene.cpp
 * @brief Implementation of the Scene class
 *
 * @date 18.01.2010
 * @author: Ole Rehmsen
 */

#include "Scene.h"

namespace Graphics {


Scene::Scene()
{
	
}


Scene::~Scene(){
}

void Scene::GetVisibleGeometry(const Camera& camera, vector<Geometry*>& visibleNodes) const {
	for (vector<Geometry*>::const_iterator i = geometry_.begin(); i != geometry_.end(); i++)
	{
		if ((*i)->visible)
			visibleNodes.push_back(*i);
	}
}

MovingGeometry* Scene::CreateMovingGeometry(const string& name, const Matrix& transform) {
	MovingGeometry* result = new MovingGeometry(name);
	result->setTransform(transform);
	geometry_.push_back(result);
	return result;
}
StaticGeometry* Scene::CreateStaticGeometry(const string& name, const Matrix& transform) {
	StaticGeometry* result = new StaticGeometry(name, transform);
	geometry_.push_back(result);
	return result;
}

};
