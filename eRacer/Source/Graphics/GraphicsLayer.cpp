#include "GraphicsLayer.h"

namespace Graphics {

GraphicsLayer* GraphicsLayer::m_pGlobalGLayer = NULL;

GraphicsLayer::GraphicsLayer()
{
	//Initialize member variables
	m_pD3D = NULL;
	m_pd3dDevice = NULL;
}

int GraphicsLayer::Init( HWND hWnd ) 
{
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here

	//Setup an example scene
	//A bunch of hard coded vertices, this sample section needs to be replaced with a better system
    CUSTOMVERTEX Vertices[] =
    {
        { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
        { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
        {  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
    };
	//Create the vertex buffer on the video card, using our Custom vertex description
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( 3 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
    {
        return E_FAIL;
    }
    //Copy the vertices into the vertex buffer
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, sizeof( Vertices ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices, sizeof( Vertices ) );
    m_pVB->Unlock();
	//Don't forget to call release() on the vertex buffer object to free the video memory when finished


    return S_OK;
}

int GraphicsLayer::RenderFrame()
{
    if( NULL == m_pd3dDevice )
        return -1;

    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );

    //Draw the current frame
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
		m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( CUSTOMVERTEX ) );
        m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
        m_pd3dDevice->EndScene();
    }

    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}

int GraphicsLayer::Shutdown()
{
	if( NULL != m_pd3dDevice)
	{
        m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}

    if( NULL != m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	if( NULL != m_pVB)
	{
        m_pVB->Release();
		m_pVB = NULL;
	}
	return S_OK;
}

void GraphicsLayer::draw(const TriMesh& mesh){
	//TODO implement
}

};
