#include "../Core/Time.h"
#include "GraphicsLayer.h"

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

int GraphicsLayer::SetCamera(const Camera& cam)
{
	m_camera = cam;

	Matrix tmpView = cam.GetViewMatrix();
	Matrix tmpProj = cam.GetProjectionMatrix();

	D3DXMATRIXA16 matView;
    D3DXMATRIXA16 matProj;

	//Copy the matrix for now
	for (int i = 0; i<16; i++) {
		matView[i] = tmpView[i];
		matProj[i] = tmpProj[i];
	}

    //D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );

	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return 0;
}

int GraphicsLayer::Init( HWND hWnd ) 
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the D3DDevice
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

	// Turn on the zbuffer
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // Turn on ambient lighting 
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
    return S_OK;
}


int GraphicsLayer::RenderFrame()
{
	// Clear the backbuffer and the zbuffer
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
	//In the future this will be done inside a loop to handle each shader/effect
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
		vector<Geometry*> visibleObjects;
		m_scene->GetVisibleNodes(m_camera, visibleObjects);
		for(vector<Geometry*>::const_iterator object = visibleObjects.begin(); 
			object!=visibleObjects.end(); object++){
			//The camera can be set here, but does not need to be
			// Meshes are divided into subsets, one for each material. Render them in a loop
			for(unsigned int i = 0; i<(*object)->Materials().size(); i++){
				m_pd3dDevice->SetMaterial( (*object)->Materials()[i]);
				m_pd3dDevice->SetTexture(0, (*object)->Textures()[i]);
				(*object)->GetMesh()->DrawSubset(i);
			}
		}
		
        // End the scene
        m_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}

int GraphicsLayer::Shutdown()
{
	//Release the Devce
    if( NULL != m_pd3dDevice )
        m_pd3dDevice->Release();
	m_pd3dDevice = NULL;

    if( NULL != m_pD3D)
        m_pD3D->Release();
	m_pD3D = NULL;

	return S_OK;
}

};