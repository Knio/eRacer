/**
 * @file Geometry.h
 * @brief Definition of the Geometry class
 *
 * @date 22.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Spatial.h"
#include <vector>
#include "Renderable.h"
#include "Model.h"


using namespace std;

namespace Graphics {

/**
 * @brief base class for different kinds of geometry
 * 
 * @see MovingGeometry
 * @see StaticGeometry
 */
class Geometry : public Spatial, public Renderable, public Model
{
public:
	/**
	 * @brief Destructor stub. Virtual so that subclasse's constructors will be called
	 *
	 */
	virtual ~Geometry();


	virtual void Draw(IDirect3DDevice9* device) const;

	/**
	 * @brief Add myself to the list
	 *
	 * @param camera
	 *			The camera to cull against - not needed here
	 * @param visibleNodes
	 * 			A vector to push this node to
	 *
	 * @see Spatial::cullRecursive
	 */
	virtual void cullRecursive(const Camera& camera, vector<const Geometry*>& visibleNodes) const;

	const Matrix& GetTransform() const { return transform_; }



protected:
	/**
	 * @brief Constructor. Only for inheriting classes because this class is abstract.
	 *
	 * @param name
	 *			a name for this node to fascilitate debugging
	 */
	Geometry(const string& name);

	void UpdateBounds();


	Matrix transform_;

};



}
