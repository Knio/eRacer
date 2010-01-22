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
#include "Camera.h"
#include "StaticGeometry.h"
#include "Scene.h"

namespace Graphics {

class GraphicsLayer
{

private:
	static GraphicsLayer *m_pGlobalGLayer;

	LPDIRECT3D9			m_pD3D;			//Used to create the D3DDevice
	LPDIRECT3DDEVICE9   m_pd3dDevice;	//Our rendering device

	Camera m_camera;

protected:
	GraphicsLayer();	//Constructor, Singleton 
	GraphicsLayer(const GraphicsLayer&);
	GraphicsLayer& operator= (const GraphicsLayer); 
	int Draw();

public:
	Scene* m_scene;
	~GraphicsLayer();	//Destructor
	int Init( HWND hWnd );

	const LPDIRECT3DDEVICE9 GetDevice() { return m_pd3dDevice; }
	int RenderFrame();
	int Shutdown();
	int SetCamera();
	int SetCamera(const Camera& cam);
	int LoadGeometryTest(StaticGeometry &geom, const char* filePath, const char* textPath); 

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