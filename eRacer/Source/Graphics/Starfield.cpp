#include "Starfield.h"
#include "GraphicsLayer.h"
namespace Graphics {

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
    stars[i].pos.x = ((float)(rand())/RAND_MAX-0.5f)*2*SIZE;
    stars[i].pos.y = ((float)(rand())/RAND_MAX-0.5f)*2*SIZE;
    stars[i].pos.z = ((float)(rand())/RAND_MAX-0.5f)*2*SIZE;
    stars[i].color = 0x00FFFFFF;
  }
  oldcamera = IDENTITY;
}
 

void Starfield::Update(const Matrix &newcamera, const Point3& pos)
{
  Matrix c1 = oldcamera;
  Matrix c2 = newcamera;
  Matrix i2;
  oldcamera = newcamera;
  D3DXMatrixInverse(&i2, NULL, &c2);
  
  if (i2._14 != 0) return;
  
  Star *starbuff;
  vb->Lock(0, 0, (void**)&starbuff, 0);

  Star t;
  for (int i=0; i<N; i++)
  {
    t = stars[i];
    //printf("%5.1f %5.1f %5.1f \n", t.pos.x, t.pos.y, t.pos.z);
    while (t.pos.x-pos.x > SIZE) t.pos.x -= 2*SIZE;
    while (t.pos.x-pos.x <-SIZE) t.pos.x += 2*SIZE;
    while (t.pos.y-pos.y > SIZE) t.pos.y -= 2*SIZE;
    while (t.pos.y-pos.y <-SIZE) t.pos.y += 2*SIZE;
    while (t.pos.z-pos.z > SIZE) t.pos.z -= 2*SIZE;
    while (t.pos.z-pos.z <-SIZE) t.pos.z += 2*SIZE;

    starbuff[2*i+0] = t;
    
    t.pos = mul1(c1, t.pos);
    t.pos = mul1(i2, t.pos);

    starbuff[2*i+1] = t;
  }
  
  vb->Unlock();
}
void Starfield::Draw(LPDIRECT3DDEVICE9 dev)
{
  dev->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY);
  dev->SetStreamSource(0, vb, 0, sizeof(Star));
  dev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  dev->DrawPrimitive(D3DPT_LINELIST,  0,   N);
  dev->DrawPrimitive(D3DPT_POINTLIST, 0, 2*N);
  
}
}