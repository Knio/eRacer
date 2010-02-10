#include "GraphicsLayer.h"

#include "Core/Time.h"
#include "Math.h"

#include <iostream>
using namespace std;

namespace Graphics {

GraphicsLayer* GraphicsLayer::m_pGlobalGLayer = NULL;

GraphicsLayer::GraphicsLayer()
{
}

GraphicsLayer::~GraphicsLayer()
{
    if (NULL != m_pGlobalGLayer) {
        delete m_pGlobalGLayer;
        m_pGlobalGLayer = NULL;
    }
}

void GraphicsLayer::SetCamera(const Camera& camera)
{
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &camera.GetProjectionMatrix() );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &camera.GetViewMatrix() );
}

int GraphicsLayer::Init( HWND hWnd ) 
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
    {
        assert(false);
        return E_FAIL;
    }

	resetPresentationParameters();
    
    
    // Create the D3DDevice
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &m_presentationParameters, &m_pd3dDevice ) ) )
    {
        assert(false);
        return E_FAIL;
    }

    //Init the font manager
    m_fontManager.Init(m_pd3dDevice);

    // Turn on the zbuffer
    assert(SUCCEEDED(m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE )));

    // Turn on ambient lighting 
    assert(SUCCEEDED(m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff )));

    //for testing, do not cull anything
    assert(SUCCEEDED(m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW)));

    assert(SUCCEEDED(m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR)));
    assert(SUCCEEDED(m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR)));
    
    // AA
    assert(SUCCEEDED(m_pd3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE)));
    

    return S_OK;
}

void GraphicsLayer::resetPresentationParameters(){
    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    ZeroMemory( &m_presentationParameters, sizeof( m_presentationParameters ) );
    m_presentationParameters.Windowed = TRUE;
    m_presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_presentationParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    m_presentationParameters.EnableAutoDepthStencil = TRUE;
    m_presentationParameters.AutoDepthStencilFormat = D3DFMT_D16;
    m_presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}


void GraphicsLayer::PreRender(){
        // Clear the backbuffer and the zbuffer
    assert(SUCCEEDED(m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 )));

    // Begin the scene
    //In the future this will be done inside a loop to handle each shader/effect
    assert(SUCCEEDED( m_pd3dDevice->BeginScene()));
}


void GraphicsLayer::RenderView(const View& view){

    SetCamera(*view.camera);

    vector<Renderable*> visibleRenderables;
    view.scene->GetVisibleRenderables(*view.camera, visibleRenderables);
    
    for (vector<Renderable*>::const_iterator renderable = visibleRenderables.begin(); 
        renderable!=visibleRenderables.end(); renderable++){
			(*renderable)->Draw(m_pd3dDevice);
    }
    
    for (vector<const Renderable*>::const_iterator renderable = view.viewDependantRenderables.begin(); 
        renderable!=view.viewDependantRenderables.end(); renderable++){
            (*renderable)->Draw(m_pd3dDevice);
    }
}

void GraphicsLayer::PostRender(){
    assert(SUCCEEDED(m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY)));

     // draw overlay
    m_fontManager.Draw();
    
    
    // End the scene
    assert(SUCCEEDED(m_pd3dDevice->EndScene()));
    
    // Present the backbuffer contents to the display

	HRESULT r = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	switch(r){
	case D3DERR_DRIVERINTERNALERROR:
		printf("driver internal error - trying to reset presentation parameters\n");
		resetDevice();
		break;
	case D3DERR_DEVICEREMOVED:
		throw runtime_error("Fatal error: The Direct3D Device has been removed");
		break;
	default:
		if(FAILED(r))
			printf("Encountered logic error 0x%x\n",(int)r);
		assert(SUCCEEDED(r));
	}
}

void GraphicsLayer::resetDevice(){
	resetPresentationParameters();
	HRESULT r = m_pd3dDevice->Reset(&m_presentationParameters);

	switch(r){
	case D3DERR_DEVICELOST:
		printf("Fatal error: The Direct3D Device has been lost");
		throw runtime_error("Fatal error: The Direct3D Device has been lost");
	case D3DERR_DEVICEREMOVED:
		printf("Fatal error: The Direct3D Device has been lost");
		throw runtime_error("Fatal error: The Direct3D Device has been removed");
	case D3DERR_DRIVERINTERNALERROR:
		printf("again driver internal error\n");
		throw runtime_error("Fatal error: Repeated driver internal error.");
	case D3DERR_OUTOFVIDEOMEMORY:
		printf("Fatal error: Out of video memory\n");
		throw runtime_error("Fatal error: Out of video memory.");
	}
}



void GraphicsLayer::WriteString(const char* msg, const char* fontName, const float &size, const Vector3 &pos, const RGB &color)
{
    m_fontManager.WriteString(msg, fontName, size, pos, color);
}

void GraphicsLayer::Shutdown()
{
    //Release the Devce
    if( NULL != m_pd3dDevice )
        m_pd3dDevice->Release();
    m_pd3dDevice = NULL;

    if( NULL != m_pD3D)
        m_pD3D->Release();
    m_pD3D = NULL;

    m_fontManager.Shutdown();
}

}