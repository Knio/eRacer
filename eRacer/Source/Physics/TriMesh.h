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

class TriMesh : public PhysicsObject {
public:
	TriMesh(ID3DXMesh& mesh);
	~TriMesh();

};

}
