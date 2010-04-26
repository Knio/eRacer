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
#include "StringSprite.h"
#include "Camera.h"

#include "DebugRenderable.h"

#define SAFE_RELEASE(p) { if(NULL != (p)) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)	{ if(NULL != (p)) { delete (p); (p)=NULL; } }


namespace Graphics {


/**
 * @brief wraps DirectX calls for flexble rendering
 */
class GraphicsLayer
{
public:
	int width, height;
	
	DebugRenderable* debugRenderable;
	ID3DXEffect* m_pEffect;       // Temporary Variable Only!! Please do not use!

private:
	Camera *camera;
	
	IDirect3DSurface9* screen;
	IDirect3DSurface9* msaasurf;
	IDirect3DSurface9* depthsurf;
	
	IDirect3DTexture9* rttex;
	IDirect3DSurface9* rtsurf;
	
	ID3DXEffect* postprocess;
	
	std::map<std::string, ID3DXEffect*> effects;

	LPDIRECT3D9						m_pD3D;				//Used to create the D3DDevice
	IDirect3DDevice9*   	m_pd3dDevice;	//Our rendering device
	D3DPRESENT_PARAMETERS m_presentationParameters;
	StringSprite* stringSprite;

public:

	IDirect3DDevice9* GetDevice() const { return m_pd3dDevice; }
	
	~GraphicsLayer();
	int Init( HWND hWnd );
	void Shutdown();

	void PreRender();
	void PostRender();
	
	void WriteString(const char* text, const char* family, int size, long x, long y, const Vector3 &color = WHITE);
	void ClearStrings();
	
	D3DMATERIAL9 DefaultMaterial(); // also a hack
	
	ID3DXEffect* GetEffect(char* file);
	
	void SetCamera(Camera& camera);
	Camera* GetCamera() { return camera; }
	
	void SetViewport(int x, int y, int w, int h);
	void ResetViewport();
	
	
	void SetTexture(UINT id, IDirect3DTexture9* tex)
	{
		static IDirect3DTexture9* oldtex[16] =  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		if (tex != oldtex[id])
			m_pd3dDevice->SetTexture(0, oldtex[id]=tex);
	}

	static GraphicsLayer& GetInstance()
	{
		static GraphicsLayer instance;
		return instance;
	}

private:
	//Singleton: make sure default/copy constructor are not auto generated
	GraphicsLayer();	 
	GraphicsLayer(const GraphicsLayer&){ assert(false); }

	void resetPresentationParameters();
	void resetDevice();
	void InvalidateDeviceObjects();
	void RestoreDeviceObjects();
	void WaitForDevice();
};

}

#endif