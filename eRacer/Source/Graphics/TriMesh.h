/**
 * @file TriMesh.h
 * @brief Definition of the TriMesh class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "../Core/types.h"
#include <vector>
#include "Renderable.h"

using namespace std;


/**
 * @brief A triangular mesh that can be drawn by Direct3D. 
 */
class TriMesh : public Renderable {
public:



private:
	ID3DXMesh* mesh_;
};


#endif /* TRIMESH_H_ */
