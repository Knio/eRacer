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

void Scene::GetVisibleNodes(const Camera& camera, vector<StaticGeometry*>& visibleNodes) const {
	visibleNodes = geometry_;
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
