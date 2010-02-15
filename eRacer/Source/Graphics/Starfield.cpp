#include "Starfield.h"
#include "GraphicsLayer.h"

namespace Graphics {

float random(float _min=0, float _max=1)
{
  float f = (float)rand()/RAND_MAX;
  return (_max-_min)*f + _min;
}

Starfield::Starfield(int n, float s) : N(n), SIZE(s), vb(NULL), stars(NULL)
{   
	GraphicsLayer*g = GraphicsLayer::GetInstance();
	assert(SUCCEEDED(g->GetDevice()->CreateVertexBuffer(
    2 * N * sizeof(Star),   
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &vb,
    NULL
  )));
  stars = new Star[N];
  for (int i=0;i<N;i++)
  {
    stars[i].pos.x = random(-1, 1)*SIZE;
    stars[i].pos.y = random(-1, 1)*SIZE;
    stars[i].pos.z = random(-1, 1)*SIZE;
    stars[i].color = D3DCOLOR_COLORVALUE(
      random(0.7, 1.0), 
      random(0.7, 1.0), 
      random(0.7, 1.0),
      1.0f
    );
    
  }
  view = new Matrix[2];
  view[0] = IDENTITY;
  view[1] = IDENTITY;
}


void Starfield::_Update()
{
  Matrix old =         view[1];
  Matrix inv = Inverse(view[0]);
  
  if (!affine(old)) return;
  if (!affine(inv)) return;
  
  // debug(old);
  // debug(inv);
  
  Star *starbuff;
  vb->Lock(0, 0, (void**)&starbuff, 0);
  
  // This needs to be done in a vertex shader
  
  /*
  Math:
    
  P1 = V1 * P
  P2 = V2 * P
  :.
  P2 = V1 * V1^-1 * V2 * P
  
  So multiply P by the old camera, then the inverse of the new camera
  */
  
  Star t;
  for (int i=0; i<N; i++)
  {
    t = stars[i];
    
    while (t.pos.x-pos.x > SIZE) t.pos.x -= 2*SIZE;
    while (t.pos.x-pos.x <-SIZE) t.pos.x += 2*SIZE;
    while (t.pos.y-pos.y > SIZE) t.pos.y -= 2*SIZE;
    while (t.pos.y-pos.y <-SIZE) t.pos.y += 2*SIZE;
    while (t.pos.z-pos.z > SIZE) t.pos.z -= 2*SIZE;
    while (t.pos.z-pos.z <-SIZE) t.pos.z += 2*SIZE;

    starbuff[2*i+0] = t;
    
    t.pos = mul1(old, t.pos);
    t.pos = mul1(inv, t.pos);

    starbuff[2*i+1] = t;
  }
  
  vb->Unlock();
}

void Starfield::Update()
{
  
  GraphicsLayer* g = GraphicsLayer::GetInstance();
  camera = g->GetCamera();
  
  // TODO rotate with bigger circular buffer
  view[1] = view[0];
  view[0] = camera->GetViewMatrix();
  
  
  pos = camera->GetPosition();
  
  _Update();
}
  
void Starfield::Draw(IDirect3DDevice9* dev) const
{
  
  dev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255,255,255,255));
  dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
  dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
  dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
  
  dev->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY);
  dev->SetStreamSource(0, vb, 0, sizeof(Star));
  dev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  dev->DrawPrimitive(D3DPT_LINELIST,  0,   N);
  dev->DrawPrimitive(D3DPT_POINTLIST, 0, 2*N);
  // draw again as points because DX doesnt like to draw zero-length lines
}
}