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
    0, //D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &vb,
    NULL
  )));
  
  Star *starbuff;
  vb->Lock(0, 0, (void**)&starbuff, 0);

  Star t;
  for (int i=0; i<N; i++)
  {
    t.pos.x = random(-1, 1)*SIZE;
    t.pos.y = random(-1, 1)*SIZE;
    t.pos.z = random(-1, 1)*SIZE;
    t.color = D3DCOLOR_COLORVALUE(
      random(0.7f, 1.0f), 
      random(0.7f, 1.0f), 
      random(0.7f, 1.0f),
      0,
    );
    
    starbuff[2*i+0] = t;
    
    t.color |= 0xff000000;
    starbuff[2*i+1] = t;
  }
  
  vb->Unlock();
  
  view = new Matrix[2];
  view[0] = IDENTITY;
  view[1] = IDENTITY;
  
  effect = GraphicsLayer::GetInstance()->GetEffect("Starfield.fx");
  effect->SetTechnique("Starfield");
  
}



  /*
  Math:
    
  P1 = V1 * P
  P2 = V2 * P
  :.
  P2 = V1 * V1^-1 * V2 * P
  
  So multiply P by the old camera, then the inverse of the new camera
  */
  

void Starfield::Update()
{
  
  GraphicsLayer* g = GraphicsLayer::GetInstance();
  camera = g->GetCamera();
  
  // TODO rotate with bigger circular buffer
  view[1] = view[0];
  view[0] = camera->GetViewMatrix();
  
  
  pos = camera->GetPosition();
  
  // _Update();
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
  
  D3DXVECTOR4 camPos;
  camPos.x = pos.x;
  camPos.y = pos.y;
  camPos.z = pos.z;
  

  effect->SetMatrix("PMatrix",  &GraphicsLayer::GetInstance()->GetCamera()->GetProjectionMatrix());
  effect->SetMatrix("V1Matrix", &view[0]);
  effect->SetMatrix("V2Matrix", &view[1]);
  effect->SetFloat ("SIZE",     SIZE);
  effect->SetVector("camPos",   &camPos);
  
  unsigned int p;
  effect->Begin(&p, 0);
  for (unsigned int i=0;i<p;i++)
  {
    effect->BeginPass(i);
    dev->DrawPrimitive(D3DPT_LINELIST,  0,   N);
    dev->DrawPrimitive(D3DPT_POINTLIST, 0, 2*N);
    effect->EndPass();
  }
  effect->End();
  // draw again as points because DX doesnt like to draw zero-length lines
}
}