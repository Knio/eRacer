/**
 * @file GraphicsLayer.h
 * @brief Definition of the GraphicsLayer class
 *
 * @date 09.01.2010
 * @author Don Ha
 */

#pragma once
#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include "Renderable.h"
#include "FontManager.h"
#include "View.h"

namespace Graphics {


/**
 * @brief wraps DirectX calls for flexble rendering
 */
class GraphicsLayer
{

private:
	static GraphicsLayer *m_pGlobalGLayer;

	LPDIRECT3D9			m_pD3D;			//Used to create the D3DDevice
	LPDIRECT3DDEVICE9   m_pd3dDevice;	//Our rendering device
	FontManager			m_fontManager;

protected:
	GraphicsLayer();	//Constructor, Singleton 
	GraphicsLayer(const GraphicsLayer&);
	GraphicsLayer& operator= (const GraphicsLayer); 

	//void RenderGeometry(const Geometry* geometry);
	//void RenderSkyBox(const Camera& camera, const Geometry& skyBox);
	void SetCamera(const Camera& camera);

public:
	Scene* m_scene;
	~GraphicsLayer();	//Destructor
	int Init( HWND hWnd );

	void WriteString(const char* msg, const char* fontName, const float &size, const Vector3 &pos, const RGB &color=WHITE);
	const LPDIRECT3DDEVICE9 GetDevice() { return m_pd3dDevice; }

	void PreRender();
	void RenderView(const View& view);
	void PostRender();
	
	
	void Shutdown();

	static GraphicsLayer *GetInstance()
	{
		if (m_pGlobalGLayer == NULL)
			m_pGlobalGLayer = new GraphicsLayer;

		return m_pGlobalGLayer;
	}
	LPDIRECT3DDEVICE9 GetDevice() const { return m_pd3dDevice; }

};

inline GraphicsLayer *GraphicsModule()
{
	return GraphicsLayer::GetInstance();
}

};

#endif