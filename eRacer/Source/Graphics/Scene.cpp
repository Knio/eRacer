/**
 * @file Scene.cpp
 * @brief Implementation of the Scene class
 *
 * @date 18.01.2010
 * @author: Ole Rehmsen
 */

#include "Scene.h"
#include "IO/IO.h"

namespace Graphics {


Scene::Scene()
	: skyBox_("SkyBox")
{

}


Scene::~Scene(){
	printf("Scene::~Scene()\n");
	
	for(vector<Geometry*>::iterator i = geometry_.begin();
		i != geometry_.end(); i++)
		delete *i;
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


void Scene::LoadSkyBox(const std::string& filename){
	const float SKYBOX_SIZE = 1000;

	//we might want to make IO use strings in future...
	IO::GetInstance()->LoadMesh(&skyBox_, filename.c_str());
	skyBox_.SetTransform(CreateMatrix(SKYBOX_SIZE,SKYBOX_SIZE,SKYBOX_SIZE));
}


const Geometry& Scene::GetSkyBox() const{
	return skyBox_;
}


};
