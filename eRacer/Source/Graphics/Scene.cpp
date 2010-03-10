/**
 * @file Scene.cpp
 * @brief Implementation of the Scene class
 *
 * @date 18.01.2010
 * @author: Ole Rehmsen
 */

#include "Scene.h"
#include "IO/IO.h"
#include "GraphicsLayer.h"

namespace Graphics {


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Add(const MeshNode& node)
{
	meshNodes_.push_back(node);
}


void Scene::GetVisibleRenderables(const Camera& camera, vector<const Renderable*>& visibleRenderables) const {
	for (vector<MeshNode*>::const_iterator meshNode = meshNodes_.begin(); 
		meshNode != meshNodes_.end(); meshNode++)
	{
		(*meshNode)->cull(camera,visibleRenderables);
	}
}

void Scene::Draw(IDirect3DDevice9* m_pd3dDevice) const
{
	Camera* cam = GraphicsLayer::GetInstance()->GetCamera();
	vector<const Renderable*> renderables;
	GetVisibleRenderables(*cam, renderables);
	
	for (vector<const Renderable*>::const_iterator i = renderables.begin(); 
		i != renderables.end(); i++){
		(*i)->Draw(m_pd3dDevice);
	}
}

};
