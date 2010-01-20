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
	 * @param visibleNodes
	 *			the list the visible geometry nodes are appended to 
	 *
	 */
	void GetVisibleNodes(const Camera& camera, vector<StaticGeometry*>& visibleNodes) const;

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
	StaticGeometry* CreateStaticGeometry(const string& name, const Matrix& transform);

	/**
	 * @brief Factory method to create new geometry in the scene
	 *
	 * @param name
	 *			a name for the geometry - for debugging
	 * @param transform
	 *			a transformation matrix for the geometry
	 * @return a pointer to the newly created geometry
	 */ 
	MovingGeometry* CreateMovingGeometry(const string& name, const Matrix& transform);

private:
	vector<StaticGeometry*> geometry_;
};


};