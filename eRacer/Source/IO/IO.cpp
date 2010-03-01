#include "IO.h"


IO* IO::g_IO = NULL;

IDirect3DTexture9* IO::_LoadTexture(const char* file)
{
	if (!file) return (IDirect3DTexture9*)-1;
	PDIRECT3DTEXTURE9 t = NULL;
	HRESULT r = D3DXCreateTextureFromFileA(
		d3dd,
		file,
		&t
	);
	if (FAILED(r)) return (IDirect3DTexture9*)-1;
	return t;
}



bool IO::_LoadMesh(const char* file, Graphics::Mesh& mesh)
{
	LPD3DXBUFFER materialsbuffer;
	//MeshStruct mesh;
	ID3DXMesh* d3dMesh;
	DWORD nMaterials;
	D3DMATERIAL9* materials;
	IDirect3DTexture9** textures;
	
	HRESULT r = D3DXLoadMeshFromX(
		file, 
		D3DXMESH_SYSTEMMEM,
		d3dd,
		NULL,
		&materialsbuffer, 
		NULL, 
		&nMaterials,
		&d3dMesh
	);
	//use exceptions!
	if (!SUCCEEDED(r))
		return false;

	D3DXMATERIAL* materialBufferPointer = ( D3DXMATERIAL* )materialsbuffer->GetBufferPointer();
	
	materials = new D3DMATERIAL9[nMaterials];
	textures  = new IDirect3DTexture9*[nMaterials];
	for(DWORD i=0; i<nMaterials; i++)
    {
		// Copy the material
		materials[i]	= materialBufferPointer[i].MatD3D;
		textures[i]	= LoadTexture(materialBufferPointer[i].pTextureFilename);
        
		// Set the ambient color for the material (D3DX does not do this)
		materials[i].Ambient = materials[i].Diffuse;
    }
	mesh.Init(d3dMesh,nMaterials,materials,textures);
    // Done with the material buffer
    materialsbuffer->Release();
	return true;
}


void IO::_FreeTexture(IDirect3DTexture9* t)
{
	if (t)	t->Release();
}

