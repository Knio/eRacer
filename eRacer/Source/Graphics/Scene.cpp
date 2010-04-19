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

void Scene::Add(const RenderableNode& node)
{
	renderableNodes_.push_back(&node);
}

void Scene::Remove(const RenderableNode& node){
	vector<const RenderableNode*>::iterator i = renderableNodes_.begin();
	while(i != renderableNodes_.end())
	{
		if(*i == &node){
			renderableNodes_.erase(i);
			break;
		}
		i++;
	}
}



void Scene::GetVisibleRenderables(const Camera& camera, vector<const Renderable*>& visibleRenderables) const {
	for (vector<const RenderableNode*>::const_iterator renderableNode = renderableNodes_.begin(); 
		renderableNode != renderableNodes_.end(); renderableNode++)
	{
		(*renderableNode)->cull(camera,visibleRenderables);
	}
}

void Scene::Draw(IDirect3DDevice9* m_pd3dDevice) const
{
	Camera* cam = GraphicsLayer::GetInstance().GetCamera();
	vector<const Renderable*> renderables;
	GetVisibleRenderables(*cam, renderables);
	
	for (vector<const Renderable*>::const_iterator i = renderables.begin(); 
		i != renderables.end(); i++){
		(*i)->Draw(m_pd3dDevice);
	}
}

};
