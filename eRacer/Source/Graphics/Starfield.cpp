#include "Starfield.h"

const float Starfield::SIZE = 1000.0;


Starfield::Starfield(LPDIRECT3DDEVICE9 d, int N) : N(N), vb(NULL), stars(NULL), dev(d)
{   
  assert(SUCCEEDED(dev->CreateVertexBuffer(
    2 * N * sizeof(Star),   
    D3DUSAGE_DYNAMIC,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &vb,
    NULL
  )));
  stars = new Star[N];
  for (int i=0;i<N;i++)
  {
    stars[i].pos.x = ((float)(rand())/RAND_MAX-0.5)*2*SIZE;
    stars[i].pos.y = ((float)(rand())/RAND_MAX-0.5)*2*SIZE;
    stars[i].pos.z = ((float)(rand())/RAND_MAX-0.5)*2*SIZE;
    stars[i].color = 0x00FFFFFF;
  }
  camera = IDENTITY;
  
}
void Starfield::Update(const Matrix &c1, const Matrix &c2, const Point3& pos)
{
  Matrix i1;
  Matrix i2;
  D3DXMatrixInverse(&i1, NULL, &c1);
  D3DXMatrixInverse(&i2, NULL, &c2);
  
  // debug(c1);
  // debug(c2);

  // debug(i1);
  // debug(i2);

  if (i1._14 != 0) return;
  if (i2._14 != 0) return;
  

  Star *starbuff;
  vb->Lock(0, 0, (void**)&starbuff, 0);
  
  
  /*
  
  W1 = C1 * p0
  W2 = C2 * p0
  
  W1 = 1 * (C1 * p0)
  W1 = C2 * C2^-1 * (C1 * p0)

  */
  
  
  
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
    
    // t.pos.x += ((float)rand())/RAND_MAX;
    // t.pos.y += ((float)rand())/RAND_MAX;
    // t.pos.z += ((float)rand())/RAND_MAX;

    starbuff[2*i+1] = t;

  }
  
  vb->Unlock();
};
void Starfield::Draw(const Matrix &newcamera, const Point3 &pos)
{
  Update(camera, newcamera, pos);
  camera = newcamera;
  dev->SetStreamSource(0, vb, 0, sizeof(Star));
  dev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  dev->DrawPrimitive(D3DPT_LINELIST, 0, N);
}


