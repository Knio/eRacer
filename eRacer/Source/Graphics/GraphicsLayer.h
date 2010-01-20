#pragma once
#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include <d3d9.h>
#include "StaticGeometry.h"

namespace Graphics {

// A structure for our custom vertex type
//Borrowed from the samples for a quick demonstration, the whole sample section
//should be replaced with programmable shaders, although I'm less certain how to 
//use them under DirectX 9.0
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};
typedef CUSTOMVERTEX Vertex;


// Our custom FVF, which describes the layout of the custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class GraphicsLayer
{
private:
	static GraphicsLayer *m_pGlobalGLayer;

	LPDIRECT3D9			m_pD3D;			//Used to create the D3DDevice
	LPDIRECT3DDEVICE9   m_pd3dDevice;	//Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB;		//Buffer to hold Vertices, stopgap for now

protected:
	GraphicsLayer();	//Constructor, Singleton 
	GraphicsLayer(const GraphicsLayer&);
	GraphicsLayer& operator= (const GraphicsLayer&); 

public:
	~GraphicsLayer() {};	//Destructor
	int Init( HWND hWnd );
	int RenderFrame();
	int Shutdown();

	static GraphicsLayer *GetGraphicsInstance()
	{
		if (m_pGlobalGLayer == NULL)
			m_pGlobalGLayer = new GraphicsLayer;

		return m_pGlobalGLayer;
	}
};

inline GraphicsLayer *Graphics()
{
	return GraphicsLayer::GetGraphicsInstance();
}

};
#endif
