/**
 * @file TriMesh.h
 * @brief Definition of the Physics::TriMesh class
 *
 * @date 31.01.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "PhysicsObject.h"


namespace Physics{

/**
 * @brief wrapper for a physsics actor that is a triangle mesh
 */
class TriMesh : public PhysicsObject {
public:
	/**
	 * @brief Constructor stub.
	 */
	TriMesh();
	
	/**
	 * @brief Destructor stub.
	 */ 
	~TriMesh();

	/**
	 * @brief Transfer mesh data from Direct3D mesh to PhysX mesh
	 *
	 * @param mesh
	 * 			the Direct3D mesh to stream the mesh data from
	 */
	void Init(ID3DXMesh& mesh);

};

}
