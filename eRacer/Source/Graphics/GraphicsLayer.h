//-----------------------------------------------------------------------------
// File: GraphicsLayer.h
// Jan 9, 2010 
//
// GraphicsLayer wraps DirectX calls for flexble rendering
// Wallsocket Studios
//-----------------------------------------------------------------------------

#pragma once
#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "Camera.h"
#include "StaticGeometry.h"
#include "Scene.h"

namespace Graphics {

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};
// Our custom FVF, which describes the layout of the custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class GraphicsLayer
{
private:
	static GraphicsLayer *m_pGlobalGLayer;

	LPDIRECT3D9			m_pD3D;			//Used to create the D3DDevice
	LPDIRECT3DDEVICE9   m_pd3dDevice;	//Our rendering device

	Camera m_camera;
	Scene m_scene;


//Test Variables
	LPDIRECT3DVERTEXBUFFER9 m_pVB;		//Buffer to hold Vertices, stopgap for now
	int m_VBSize;						//Number of elements in the Vertex Buffer
	LPD3DXMESH          m_pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*       m_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9* m_pMeshTextures; // Textures for our mesh
	DWORD               m_dwNumMaterials;   // Number of mesh materials

protected:
	GraphicsLayer();	//Constructor, Singleton 
	GraphicsLayer(const GraphicsLayer&);
	GraphicsLayer& operator= (const GraphicsLayer); 
	int Draw();

public:
	~GraphicsLayer();	//Destructor
	int Init( HWND hWnd );
	//int DrawRaw(CUSTOMVERTEX vertexBuf[], int vertexBufSize); //For testing

	int RenderFrame();
	int RenderFrame(const StaticGeometry& r);
	int Shutdown();
	int SetCamera();
	int SetCamera(const Camera& cam);
	HRESULT LoadGeometryTest(LPD3DXMESH& pMesh, D3DMATERIAL9*& pMeshMaterials, LPDIRECT3DTEXTURE9*& pMeshTextures, 
		DWORD& dwNumMaterials, const char* meshPath, const char* texPath );

	static GraphicsLayer *GetGraphicsInstance()
	{
		if (m_pGlobalGLayer == NULL)
			m_pGlobalGLayer = new GraphicsLayer;

		return m_pGlobalGLayer;
	}
};

inline GraphicsLayer *GraphicsModule()
{
	return GraphicsLayer::GetGraphicsInstance();
}

};

#endif