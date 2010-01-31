#include "GraphicsLayer.h"

#include "Core/Time.h"
#include "Math.h"

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
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &camera.GetViewMatrix() );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &camera.GetProjectionMatrix() );
}

int GraphicsLayer::Init( HWND hWnd ) 
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
        assert(false);
		return E_FAIL;
	}

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
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pd3dDevice ) ) )
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

	
	return S_OK;
}


void GraphicsLayer::RenderFrame(const Camera& camera, const Scene& scene)
{
	// Clear the backbuffer and the zbuffer
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	SetCamera(camera);

	vector<Geometry*> visibleGeometry;
	scene.GetVisibleGeometry(camera, visibleGeometry);

    // Begin the scene
	//In the future this will be done inside a loop to handle each shader/effect
    assert(SUCCEEDED( m_pd3dDevice->BeginScene()));


	for(vector<Geometry*>::const_iterator geometry = visibleGeometry.begin(); 
		geometry!=visibleGeometry.end(); geometry++){
			RenderGeometry(*geometry);
	}
	RenderSkyBox(camera, scene.GetSkyBox());

	m_fontManager.Draw();
    // End the scene
    m_pd3dDevice->EndScene();

    // Present the backbuffer contents to the display
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void GraphicsLayer::WriteString(const char* msg, const char* fontName, const float &size, const Vector3 &pos, const Vector3 &color)
{
	m_fontManager.WriteString(msg, fontName, size, pos, color);
}

void GraphicsLayer::RenderGeometry(const Geometry* geometry){
	assert(NULL != geometry);

	//there need to be the same number of textures and materials
	assert(geometry->Textures().size()==geometry->Materials().size());
	// Meshes are divided into subsets, one for each material. Render them in a loop
	
	// set the transform
	// TODO unset it after!
	// is this even the right matrix?
	m_pd3dDevice->SetTransform(  D3DTS_WORLDMATRIX(0), &(geometry->GetTransform()) );
	

	for(unsigned int i = 0; i<geometry->Materials().size(); i++){
		m_pd3dDevice->SetMaterial( geometry->Materials()[i]);
		m_pd3dDevice->SetTexture(0, geometry->Textures()[i]);
		
		//make sure the mesh has been initialized at this point
		assert(NULL != geometry);
		assert(NULL != geometry->GetMesh());

		geometry->GetMesh()->DrawSubset(i);
	}
}

void GraphicsLayer::RenderSkyBox(const Camera& camera, const Geometry& skyBox){

	//there need to be the same number of textures and materials
	assert(skyBox.Textures().size()==skyBox.Materials().size());
	// Meshes are divided into subsets, one for each material. Render them in a loop
	
	// set the transform

	Matrix transform = skyBox.GetTransform();
	transform*=CreateMatrix(camera.GetPosition());
	m_pd3dDevice->SetTransform(  D3DTS_WORLDMATRIX(0), &transform );
	

	for(unsigned int i = 0; i<skyBox.Materials().size(); i++){
		m_pd3dDevice->SetMaterial( skyBox.Materials()[i]);
		m_pd3dDevice->SetTexture(0, skyBox.Textures()[i]);
		
		//make sure the mesh has been initialized at this point
		assert(NULL != skyBox.GetMesh());

		skyBox.GetMesh()->DrawSubset(i);
	}
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

};