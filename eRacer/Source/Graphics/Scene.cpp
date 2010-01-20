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
void Scene::GetVisibleNodes(const Camera& camera, vector<StaticGeometry*>& visibleNodes) const {assert(false);}
MovingGeometry* Scene::CreateMovingGeometry(const Matrix &transform) {assert(false); return 0;}
StaticGeometry* Scene::CreateStaticGeometry(const Matrix& transform) {assert(false); return 0;}

Scene::~Scene(){
}



};
