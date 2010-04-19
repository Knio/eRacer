#include "DebugRenderable.h"
#include "GraphicsLayer.h"

namespace Graphics {



DebugRenderable::DebugRenderable() : nLines(0), nTriangles(0)
{
  assert(SUCCEEDED(GraphicsLayer::GetInstance().GetDevice()->CreateVertexBuffer(
    MAX_OBJ * sizeof(Line),
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &lines,
    NULL
  )));

  assert(SUCCEEDED(GraphicsLayer::GetInstance().GetDevice()->CreateVertexBuffer(
    MAX_OBJ * sizeof(Triangle),
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
    D3DFVF_XYZ | D3DFVF_DIFFUSE,
    D3DPOOL_DEFAULT,
    &triangles,
    NULL
  )));
  
  
  
}

void DebugRenderable::AddNormal(const Point3& start, const Vector3& normal, DWORD color)
{
  AddLine(start, start+normal, color);
}

void DebugRenderable::AddLine(const Point3& start, const Point3 &end, DWORD color)
{
  Line *buff;
  assert(SUCCEEDED(lines->Lock(sizeof(Line)*nLines, sizeof(Line), (void**)&buff, D3DLOCK_NOOVERWRITE)));
  
  *buff = Line(Vertex(start, color), Vertex(end, color));
  
  assert(SUCCEEDED(lines->Unlock()));
  nLines++;
}  

// void DebugRenderable::AddTriangle(Point3 a, Point3 b, Point3 c)
// {
  
//   Triangle *buff;
//   assert(SUCCEEDED(triangles->Lock(sizeof(Triangle)*nTriangles, sizeof(Triangle), (void**)&buff, D3DLOCK_NOOVERWRITE));
  
//   *buf = Triangle(Vertex(a, _WHITE), Vertex(b, _WHITE), Vertex(c, _WHITE));
  
//   assert(SUCCEEDED(triangles->Unlock()));
//   nTriangles++;  

// }

void DebugRenderable::Clear()
{
  nLines = 0;
}

void DebugRenderable::Draw(IDirect3DDevice9* dev) const 
{
  
  dev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255,255,255,255));
  dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
  dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
  dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
  
  dev->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY);
  dev->SetStreamSource(0, lines, 0, sizeof(Vertex));
  dev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  dev->DrawPrimitive(D3DPT_LINELIST,  0,   nLines);
  
  dev->SetStreamSource(0, triangles, 0, sizeof(Vertex));
  dev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
  dev->DrawPrimitive(D3DPT_TRIANGLELIST,  0,   nTriangles);
  
}

}