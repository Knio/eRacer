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
#include "StaticMeshNode.h"
#include "MovingMeshNode.h"


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
	 * @param renderables
	 *			the vector the visible renderables are appended to 
	 *
	 */
	void GetVisibleRenderables(const Camera& camera, vector<Renderable*>& visibleRenderables) const;

	/**
	 * @brief Factory method to create new non-moving mesh nodes in the scene
	 *
	 * Static mesh nodes do not change their transform or meshes over time.
	 * This is useful for optimization of the culling process.
	 * 
	 * @param name
	 *			a name for the mesh node - for debugging
	 * @param transform
	 *			a transformation matrix for the node
	 * @return a pointer to the newly created node
	 *
	 * @see CreateMovingMeshNode
	 */ 
	StaticMeshNode* CreateStaticMeshNode(const string& name, const Matrix& transform=IDENTITY);

	/**
	 * @brief Factory method to create new moving mesh nodes in the scene
	 *
	 * @param name
	 *			a name for the mesh node - for debugging
	 * @param transform
	 *			a transformation matrix for the node
	 * @return a pointer to the newly created node
	 *
	 * @see CreateStaticMeshNode
	 */ 
	MovingMeshNode* CreateMovingMeshNode(const string& name, const Matrix& transform=IDENTITY);
	
	void Draw(IDirect3DDevice9* m_pd3dDevice) const;
	

private:
	vector<MeshNode*> 	meshNodes_;
};


};