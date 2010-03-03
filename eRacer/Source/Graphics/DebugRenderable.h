#ifndef DEBUGRENDERABLE_H_
#define DEBUGRENDERABLE_H_

#include "Core/Math.h"
#include "Renderable.h"

#define MAX_OBJ (1<<15)

#define _WHITE D3DCOLOR_COLORVALUE(1,1,1,1)

namespace Graphics {

class DebugRenderable : public Renderable
{
  
  struct Vertex {
    Point3 pos;
    DWORD color;
    Vertex(const Point3& p, const DWORD c) : pos(p), color(c) {}
  };
  struct Line
  {
    Vertex a,b;
    Line(const Vertex& a, const Vertex& b) : a(a), b(b) {}
  };
  struct Triangle
  {
    Vertex a,b,c;
  };
        

  
  LPDIRECT3DVERTEXBUFFER9 lines;
  LPDIRECT3DVERTEXBUFFER9 triangles;
  UINT nLines, nTriangles;
  
  
  
public:
  DebugRenderable();

  void AddNormal(Point3 start, Vector3 norm, DWORD color=_WHITE);
  void AddLine(Point3 start, Point3 end, DWORD color=_WHITE);  
  // void AddTriangle(Point3 a, Point3 b, Point3 c);
  
  
  virtual void Draw(IDirect3DDevice9*) const;
};


}





#endif
