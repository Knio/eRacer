#include "IO.h"
#include <cstdio>


IO* IO::g_IO = NULL;


LPDIRECT3DTEXTURE9 IO::_LoadTexture(const char* file)
{
	if (!file) return NULL;
	PDIRECT3DTEXTURE9 t = NULL;
	HRESULT r = D3DXCreateTextureFromFileA(
		d3dd,
		file,
		&t
	);
	if (FAILED(r)) return (PDIRECT3DTEXTURE9)-1;
	return t;
}



Mesh IO::_LoadMesh(const char* file)
{
	LPD3DXBUFFER materialsbuffer;
	Mesh mesh;
	HRESULT r = D3DXLoadMeshFromX(
		file, 
		D3DXMESH_SYSTEMMEM,
		d3dd,
		NULL,
		&materialsbuffer, 
		NULL, 
		&mesh.n,
		&mesh.mesh
	);
	if (!SUCCEEDED(r))
	{
		mesh.n = -1;
		return mesh;
	}

	D3DXMATERIAL* m1 = ( D3DXMATERIAL* )materialsbuffer->GetBufferPointer();
	mesh.materials = new D3DMATERIAL9[mesh.n];
	mesh.textures  = new PDIRECT3DTEXTURE9[mesh.n];
	for(DWORD i=0; i<mesh.n; i++)
    {
		// Copy the material
		mesh.materials[i]	= m1[i].MatD3D;
		mesh.textures[i]	= LoadTexture(m1[i].pTextureFilename);
        
		// Set the ambient color for the material (D3DX does not do this)
		mesh.materials[i].Ambient = mesh.materials[i].Diffuse;
    }
    // Done with the material buffer
    materialsbuffer->Release();
	return mesh;
}


void IO::_FreeTexture(LPDIRECT3DTEXTURE9 t)
{
	if (t)	t->Release();
}
void IO::_FreeMesh(Mesh &m)
{
	delete [] m.materials;
	delete [] m.textures;
	m.mesh->Release();
}

// This could probably be done by Geometry
void IO::_SetMesh(Graphics::Geometry *geom, Mesh &mesh)
{
	// TODO clear these std::vectors?
	assert(!geom->Materials().size());
	assert(!geom->Textures().size());

	geom->SetMesh(mesh.mesh);
	for(DWORD i=0; i<mesh.n; i++)
	{
		geom->Materials().push_back(&mesh.materials[i]);
		geom->Textures().push_back(mesh.textures[i]);
	}
}

