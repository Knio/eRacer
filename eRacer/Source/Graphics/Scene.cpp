/**
 * @file Scene.cpp
 * @brief Implementation of the Scene class
 *
 * @date 18.01.2010
 * @author: Ole Rehmsen
 */

#include "Scene.h"
#include "IO/IO.h"
#include "SkyBox.h"

namespace Graphics {


Scene::Scene()
{

}


Scene::~Scene(){
	for(vector<MeshNode*>::iterator i = meshNodes_.begin();
		i != meshNodes_.end(); i++)
		delete *i;
	for(vector<Renderable*>::iterator i = renderables_.begin();
		i != renderables_.end(); i++)
		delete *i;
}

void Scene::GetVisibleRenderables(const Camera& camera, vector<Renderable*>& visibleRenderables) const {
	for (vector<MeshNode*>::const_iterator i = meshNodes_.begin(); i != meshNodes_.end(); i++)
	{
		if ((*i)->visible)
			visibleRenderables.push_back(*i);
	}
	visibleRenderables.insert(visibleRenderables.end(), renderables_.begin(), renderables_.end());
}

MovingMeshNode* Scene::CreateMovingGeometry(const string& name, const Matrix& transform) {
	MovingMeshNode* result = new MovingMeshNode(name);
	result->SetTransform(transform);
	meshNodes_.push_back(result);
	return result;
}
StaticMeshNode* Scene::CreateStaticGeometry(const string& name, const Matrix& transform) {
	StaticMeshNode* result = new StaticMeshNode(name, transform);
	meshNodes_.push_back(result);
	return result;
}


void Scene::LoadSkyBox(const std::string& filename){
	const float SKYBOX_SIZE = 1000;

	SkyBox* skyBox = new SkyBox();

	//we might want to make IO use strings in future...
	IO::GetInstance()->LoadMesh(skyBox, filename.c_str());
	//skyBox_.SetTransform(CreateMatrix(SKYBOX_SIZE,SKYBOX_SIZE,SKYBOX_SIZE));
	renderables_.push_back(skyBox);
}

/*
const Geometry& Scene::GetSkyBox() const{
	return skyBox_;
}
*/

};
