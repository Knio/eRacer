#include "GraphicsLayer.h"

#include "Core/Time.h"
#include "Math.h"
#include "Game/Game.h"
#include "Core/Event.h"



#include "Core/Consts.h"
extern Constants CONSTS;

#include <iostream>
using namespace std;

namespace Graphics {

GraphicsLayer::GraphicsLayer()
  : debugRenderable(NULL),
	m_pEffect(NULL),
	camera(NULL),
	screen(NULL),
	msaasurf(NULL),
	depthsurf(NULL),
	m_pD3D(NULL),
	m_pd3dDevice(NULL),
	stringSprite(NULL)
{
}

GraphicsLayer::~GraphicsLayer()
{
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
    
	RestoreDeviceObjects();
	
	D3DMATERIAL9 m = DefaultMaterial();
    m_pd3dDevice->SetMaterial(&m);
    
    
    stringSprite = new StringSprite();
    //debugRenderable = new DebugRenderable();
    
    return S_OK;
}

void GraphicsLayer::Shutdown()
{
	//SAFE_DELETE(debugRenderable);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(screen);
	SAFE_RELEASE(msaasurf);
	SAFE_RELEASE(depthsurf);
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	SAFE_DELETE(stringSprite);
}

void GraphicsLayer::resetPresentationParameters(){
    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    ZeroMemory( &m_presentationParameters, sizeof( m_presentationParameters ) );
    m_presentationParameters.Windowed = CONSTS.WINDOWED;
    m_presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_presentationParameters.MultiSampleQuality = 0;
    m_presentationParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    m_presentationParameters.BackBufferCount = 0;
    m_presentationParameters.EnableAutoDepthStencil = TRUE;
    m_presentationParameters.AutoDepthStencilFormat = D3DFMT_D16;
    m_presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    m_presentationParameters.hDeviceWindow = Game::GetInstance()->hwnd;
    
    D3DDISPLAYMODE mode, bestMode; //bestMode will be 59hz or 60hz, either will do
    for (UINT i=0; i<m_pD3D->GetAdapterModeCount(0, D3DFMT_R5G6B5); i++)
    { 
        assert(SUCCEEDED(m_pD3D->EnumAdapterModes(
            D3DADAPTER_DEFAULT,
            D3DFMT_R5G6B5,
            i,
            &mode
        )));
		if(mode.RefreshRate == 59 || mode.RefreshRate == 60){
			bestMode = mode;
		}
        // printf("%4dx%4dx%2d\n", mode.Width, mode.Height, mode.RefreshRate);
    }
    
    if(CONSTS.WINDOWED)
    {
        m_presentationParameters.FullScreen_RefreshRateInHz = 0;
        SetWindowLong(Game::GetInstance()->hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(Game::GetInstance()->hwnd, HWND_TOP, 0, 0, CONSTS.SCREEN_WIDTH, CONSTS.SCREEN_HEIGHT,
            SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else
    {
        m_presentationParameters.BackBufferWidth    = bestMode.Width;
        m_presentationParameters.BackBufferHeight   = bestMode.Height;
        m_presentationParameters.FullScreen_RefreshRateInHz = bestMode.RefreshRate;
        m_presentationParameters.BackBufferFormat   = bestMode.Format;
        SetWindowLong(Game::GetInstance()->hwnd, GWL_STYLE, WS_POPUPWINDOW);
        SetWindowPos(Game::GetInstance()->hwnd, HWND_TOP, 0, 0, 0, 0,
            SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    
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

void GraphicsLayer::ResetViewport(){
    SetViewport(0,0,width,height);
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

	stringSprite->Draw(m_pd3dDevice);
    stringSprite->Clear();
    // draw debug
    //debugRenderable->Draw(m_pd3dDevice);
    //debugRenderable->Clear();
        
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
	case D3DERR_DEVICELOST:
		WaitForDevice();
		break;
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

		// assert(SUCCEEDED(r));
	}
}

void GraphicsLayer::WriteString(const char* text, const char* family, int size, long x, long y, const Vector3 &color){
    assert(NULL != stringSprite);
    stringSprite->Write(text, family, size, x, y, color);
}

void GraphicsLayer::ClearStrings(){
    stringSprite->Clear();
    //debugRenderable->Clear();
}

void GraphicsLayer::InvalidateDeviceObjects(){
    SAFE_RELEASE(screen);
	SAFE_RELEASE(msaasurf);
	SAFE_RELEASE(depthsurf);
}

void GraphicsLayer::RestoreDeviceObjects(){
    // create a new surface
    // http://www.borgsoft.de/renderToSurface.html
    cout << width << height << endl;
    HRESULT r = m_pd3dDevice->CreateRenderTarget(
        width, height,
        D3DFMT_A8R8G8B8,
        D3DMULTISAMPLE_4_SAMPLES, 0,
        false,
        &msaasurf,
        NULL
    );
    cout << r << endl;
    assert(SUCCEEDED(r));
    
    // create a depth buffer to go with it
    r = m_pd3dDevice->CreateDepthStencilSurface(
        width, height,
        D3DFMT_D16,
        D3DMULTISAMPLE_4_SAMPLES, 0,
        TRUE,
        &depthsurf,
        NULL
    );
    
    assert(SUCCEEDED(r));
}


void GraphicsLayer::WaitForDevice(){
	cout << "Direct3D Device lost - waiting for restore." << endl;
	InvalidateDeviceObjects();
	do {
		MSG msg;
		if(GetMessage(&msg, NULL,0,0))
			DispatchMessage(&msg);
		else
		{
			EVENT(QuitEvent());
			return;
		}
	} while( m_pd3dDevice->TestCooperativeLevel() != D3DERR_DEVICENOTRESET );
	cout << "Restored. Direct3D Device needs to reset." << endl;
	resetDevice();
	RestoreDeviceObjects();
	cout << "Direct3D Device has been reset." <<endl;
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
    default:
        cout << "Encountered logic error: "<< r << endl;
        assert(SUCCEEDED(r));    
	}
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
}