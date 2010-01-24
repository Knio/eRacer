/**
 * @file Scene.cpp
 * @brief Implementation of the Scene class
 *
 * @date 18.01.2010
 * @author: Ole Rehmsen
 */

#include "Scene.h"
#include "../IO/IO.h"

namespace Graphics {


Scene::Scene()
{
	const float SKYBOX_SIZE = 3;
		skyBox_ = new MovingGeometry("SkyBox");

		IO::GetInstance()->LoadMesh(skyBox_, "Resources/Models/skybox.x");
		skyBox_->SetTransform(CreateMatrix(SKYBOX_SIZE,SKYBOX_SIZE,SKYBOX_SIZE));
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
	result->SetTransform(transform);
	geometry_.push_back(result);
	return result;
}
StaticGeometry* Scene::CreateStaticGeometry(const string& name, const Matrix& transform) {
	StaticGeometry* result = new StaticGeometry(name, transform);
	geometry_.push_back(result);
	return result;
}


const Geometry& Scene::GetSkyBox() const{
	return *skyBox_;
}


};
