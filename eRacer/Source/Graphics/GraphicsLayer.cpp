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

void GraphicsLayer::SetCamera(Camera& cam)
{
    camera = &cam;
    
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION,   &cam.GetProjectionMatrix() );
    m_pd3dDevice->SetTransform( D3DTS_VIEW,         &cam.GetViewMatrix() );

    // HACK!
    // In the future this will be done inside a loop to handle each shader/effect
	D3DXMATRIXA16 viewMat = cam.GetViewMatrix();
	D3DXMATRIXA16 projMat = cam.GetProjectionMatrix();
	HRESULT hr;
	hr = m_pEffect->SetMatrix( "g_ViewMatrix", &viewMat );
	hr = m_pEffect->SetMatrix( "g_ProjectionMatrix", &projMat );
	hr = m_pEffect->SetTechnique( "RenderSceneWithTextureDefault" );
}

ID3DXEffect* GraphicsLayer::GetEffect(char* file)
{
    std::string path = "Source/Shaders/";
    path += file;
    if (!effects.count(path))
    {
        LPD3DXBUFFER buffer;
        assert(SUCCEEDED(D3DXCreateBuffer(
            10240,
            &buffer
        )));
        
        DWORD flags = D3DXFX_NOT_CLONEABLE;
        #ifdef _DEBUG
            flags |= D3DXSHADER_DEBUG;
        #endif
        
        ID3DXEffect* effect;
        if (!SUCCEEDED(D3DXCreateEffectFromFile(
            m_pd3dDevice, 
            path.c_str(), 
            NULL, NULL, 
            flags,
            NULL,
            &effect, 
            &buffer
        )))
        {
            char* errors = (char*)buffer->GetBufferPointer();
            printf(
                "Error loading effect %s:\n%s", 
                path.c_str(), errors
            );
            throw runtime_error(errors);
        }
        effects[path] = effect;
    }    
    return effects[path];
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
	assert(SUCCEEDED(m_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP)));
	assert(SUCCEEDED(m_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP)));
    
    // AA
    assert(SUCCEEDED(m_pd3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE)));
    
	//Shaders
	
    m_pEffect = GetEffect("BasicHLSL.fx");

    // Set effect variables as needed
    D3DXCOLOR colorMtrlDiffuse( 1.0f, 1.0f, 1.0f, 1.0f );
    D3DXCOLOR colorMtrlAmbient( 1.0f, 1.0f, 1.0f, 0 );
    m_pEffect->SetValue( "g_MaterialAmbientColor", &colorMtrlAmbient, sizeof( D3DXCOLOR ) );
    m_pEffect->SetValue( "g_MaterialDiffuseColor", &colorMtrlDiffuse, sizeof( D3DXCOLOR ) );

	D3DXCOLOR colorMtrlTint( 1.0f, 1.0f, 1.0f, 1.0f );
	m_pEffect->SetValue( "g_ColorTint", &colorMtrlTint, sizeof( D3DXCOLOR ) );

    // save the screen surface
    m_pd3dDevice->GetRenderTarget(0, &screen);
    
    
    D3DSURFACE_DESC desc;
    screen->GetDesc(&desc);
    
    width   = desc.Width;
    height  = desc.Height;

    
    // create a new surface
    // http://www.borgsoft.de/renderToSurface.html
    assert(SUCCEEDED(m_pd3dDevice->CreateRenderTarget(
        desc.Width, desc.Height,
        D3DFMT_A8R8G8B8,
        D3DMULTISAMPLE_4_SAMPLES, 0,
        false,
        &msaasurf,
        NULL
    )));
    
    // create a depth buffer to go with it
    assert(SUCCEEDED(m_pd3dDevice->CreateDepthStencilSurface(
        desc.Width, desc.Height,
        D3DFMT_D16,
        D3DMULTISAMPLE_4_SAMPLES, 0,
        TRUE,
        &depthsurf,
        NULL
    )));
    D3DMATERIAL9 m = DefaultMaterial();
    m_pd3dDevice->SetMaterial(&m);
    
    
    debugRenderable = new DebugRenderable();
    
    return S_OK;
}

D3DMATERIAL9 GraphicsLayer::DefaultMaterial()
{
    //set a default material so that even stuff without material or shaders renders
    D3DMATERIAL9 material;

    // Set the RGBA for diffuse reflection.
    material.Diffuse.r = 1.0f;
    material.Diffuse.g = 1.0f;
    material.Diffuse.b = 1.0f;
    material.Diffuse.a = 1.0f;
    
    // Set the RGBA for ambient reflection.
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    
    // Set the color and sharpness of specular highlights.
    material.Specular.r = 1.0f;
    material.Specular.g = 1.0f;
    material.Specular.b = 1.0f;
    material.Specular.a = 1.0f;
    material.Power = 50.0f;
    
    // Set the RGBA for emissive color.
    material.Emissive.r = 0.0f;
    material.Emissive.g = 0.0f;
    material.Emissive.b = 0.0f;
    material.Emissive.a = 0.0f;
    
    return material;
}

void GraphicsLayer::resetPresentationParameters(){
    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    ZeroMemory( &m_presentationParameters, sizeof( m_presentationParameters ) );
    m_presentationParameters.Windowed = TRUE;
    m_presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_presentationParameters.MultiSampleQuality = 0;
    m_presentationParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    m_presentationParameters.EnableAutoDepthStencil = TRUE;
    m_presentationParameters.AutoDepthStencilFormat = D3DFMT_D16;
    m_presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void GraphicsLayer::SetViewport(int x, int y, int w, int h)
{
    D3DVIEWPORT9 vp;
    vp.X = x;
    vp.Y = y;
    vp.Width = w;
    vp.Height = h;
    vp.MinZ = 0.0;
    vp.MaxZ = 1.0;
    
    m_pd3dDevice->SetViewport(&vp);
}

void GraphicsLayer::PreRender(){
    
    // render to offscreen surface
    assert(SUCCEEDED(m_pd3dDevice->SetRenderTarget(0, msaasurf)));
    assert(SUCCEEDED(m_pd3dDevice->SetDepthStencilSurface(depthsurf)));
    
    // Clear the backbuffer and the zbuffer
    assert(SUCCEEDED(m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 )));

    // Begin the scene
    //In the future this will be done inside a loop to handle each shader/effect
    assert(SUCCEEDED( m_pd3dDevice->BeginScene()));
    assert(SUCCEEDED(m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY)));
}

void GraphicsLayer::PostRender(){

    // draw debug
    debugRenderable->Draw(m_pd3dDevice);
    debugRenderable->Clear();
    
     // draw overlay
    m_fontManager.Draw();
    
    // do postprocessing here
    
    // copy msaasurf back to the screen
    assert(SUCCEEDED(m_pd3dDevice->SetRenderTarget(0, screen)));
    IDirect3DSurface9* backBuffer = NULL;
    assert(SUCCEEDED(m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer)));
    assert(SUCCEEDED(m_pd3dDevice->StretchRect(msaasurf, NULL, backBuffer, NULL, D3DTEXF_LINEAR)));
    backBuffer->Release();
    
    // End the scene
    assert(SUCCEEDED(m_pd3dDevice->EndScene()));

    // TODO: performance: Calling Present() later instead of now will get us 20% more CPU time
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



void GraphicsLayer::WriteString(const char* msg, const char* fontName, int size, const Vector3 &pos, const RGB &color)
{
    m_fontManager.WriteString(msg, fontName, size, pos, color);
}

void GraphicsLayer::Shutdown()
{
    if( NULL != m_pEffect)
        m_pEffect->Release();
    m_pEffect = NULL;

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