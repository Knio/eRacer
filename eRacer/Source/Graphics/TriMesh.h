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

using namespace std;

struct CUSTOMVERTEX;

/**
 * @brief A triangular mesh that can be drawn by Direct3D. 
 */
class TriMesh {
public:



private:
	vector<CUSTOMVERTEX> vertices_;
	vector<unsigned short> indices_;

	IDirect3DVertexBuffer9* vertexBuffer_;
	IDirect3DIndexBuffer9* indexBuffer_;
};


#endif /* TRIMESH_H_ */
