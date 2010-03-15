/**
 * @file Scene.h
 * @brief Definition of the Scene class
 *
 * @date 18.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include <vector>
#include "Camera.h"
#include "Renderable.h"
#include "MeshNode.h"


using namespace std;

namespace Graphics {


/**
 * @brief Provides an abstraction for whatever internal organisation of the scene may be. 
 */
class Scene : public Renderable
{
public:
	/**
	 * @brief Constructor stub.
	 */
	Scene();

	/**
	 * @brief Destructor stub.
	 */
	virtual ~Scene();

	/**
	 * @brief Fill a vector with all geometry nodes in the scene visible with a given camera.
	 *
	 * This method will just append pointers to the vectors, not checking whether it was empty before.
	 *
	 * @param camera 
	 *			a camera defining the view frustum against which the scene is culled
	 * @param visibleRenderables
	 *			the vector the visible renderables are appended to 
	 *
	 */
	void GetVisibleRenderables(const Camera& camera, vector<const Renderable*>& visibleRenderables) const;
	void Add(const RenderableNode& node);
	void Draw(IDirect3DDevice9* m_pd3dDevice) const;
	

private:
	vector<const RenderableNode*> renderableNodes_;
};


};