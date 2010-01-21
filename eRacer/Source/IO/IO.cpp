#include "IO.h"

PDIRECT3DTEXTURE9 IO::LoadTexture(const char* file)
{
	if (!file) return NULL;
	PDIRECT3DTEXTURE9 t = NULL;
	HRESULT r = D3DXCreateTextureFromFileA(
		d3dd,
		file, 
		&t
	);
	return t;
}

int IO::LoadMesh(Graphics::StaticGeometry &geom, const char* file)
{
	LPD3DXBUFFER materialsbuffer;
	LPD3DXMESH mesh;
	DWORD nmaterials;
	HRESULT r = D3DXLoadMeshFromX(
		file, 
		D3DXMESH_SYSTEMMEM,
		d3dd,
		NULL,
		&materialsbuffer, 
		NULL, 
		&nmaterials,
		&mesh
	);
	if (r != D3D_OK) assert(false);
	
	D3DXMATERIAL* m1 = ( D3DXMATERIAL* )materialsbuffer->GetBufferPointer();
	D3DMATERIAL9* m2 = new D3DMATERIAL9[nmaterials];
	PDIRECT3DTEXTURE9 t;
	for(DWORD i=0; i<nmaterials; i++)
    {
		// Copy the material
		m2[i] = m1[i].MatD3D;
		t = LoadTexture(m1[i].pTextureFilename);
        
		// Set the ambient color for the material (D3DX does not do this)
        m2[i].Ambient = m2[i].Diffuse;

		geom.Materials().push_back(&m2[i]);
		geom.Textures() .push_back(t);

    }

    // Done with the material buffer
    materialsbuffer->Release();

	geom.SetMesh(mesh);
	return 0;
};
