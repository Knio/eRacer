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
#include "StaticGeometry.h"
#include "MovingGeometry.h"


using namespace std;

namespace Graphics {


/**
 * @brief Provides an abstraction for whatever internal organisation of the scene may be. 
 */
class Scene
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
	 * @brief Factory method to create new non-moving geometry in the scene
	 *
	 * Static geometry does not change its transform or its mesh over time.
	 * This is useful for optimization of the culling process.
	 * 
	 * @param name
	 *			a name for the geometry - for debugging
	 * @param transform
	 *			a transformation matrix for the geometry
	 * @return a pointer to the newly created geometry
	 *
	 * @see CreateGeometry
	 */ 
	StaticGeometry* CreateStaticGeometry(const string& name, const Matrix& transform=IDENTITY);

	/**
	 * @brief Factory method to create new geometry in the scene
	 *
	 * @param name
	 *			a name for the geometry - for debugging
	 * @param transform
	 *			a transformation matrix for the geometry
	 * @return a pointer to the newly created geometry
	 */ 
	MovingGeometry* CreateMovingGeometry(const string& name, const Matrix& transform=IDENTITY);

	void LoadSkyBox(const std::string& filename);

	//const Geometry& GetSkyBox() const;
	
	void AddRenderable(Renderable* r) { renderables_.push_back(r); }
	//const vector<Renderable*> GetRenderables() const { return renderable_; }

private:
	vector<Geometry*> 	geometries_;
	vector<Renderable*> renderables_;
	//MovingGeometry 			skyBox_;
};


};