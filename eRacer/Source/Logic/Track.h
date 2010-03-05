#include "Math.h"
#include <vector>
#include <iostream>
#include "Graphics/GraphicsLayer.h"
using namespace std;

// TODO namespace

struct Frame
{
  Point3  position;
  Vector3 up;
  Vector3 fw;
  float dist;
  Frame(const Point3& p = ORIGIN, const Vector3& u=Y, const Vector3& f=X) : 
    position(p), 
    up(u), 
    fw(f) 
    { }
};

#define Vertex_Format (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0))
struct TrackVertex
{
  Point3  position;
  Vector3 normal;
  float tu, tv;
  TrackVertex(const Point3 &p=ORIGIN, const Vector3 &n=Y, float tu=0.f, float tv=0.f) : 
    position(p),
    normal(n),
    tu(tu),
    tv(tv)
    { }
};


class Track
{
  vector<Frame>  track;
  
  float dist;
  std::map<float, int> arclen;

public:
  Track()
  {
    
  }
  
  float GetTotalDist() { return dist; }
  
  void Add(const Frame &f);
  
  Frame GetFrame(float d);
  
  Point3 GetPositionAt(float d);
  Vector3 GetNormalAt(float d);
  
  void Subdivide(int NSUBDIV=5);
  
  ID3DXMesh* CreateMesh(const vector<TrackVertex>& profile);
};

inline void Track::Add(const Frame &f)
{
   track.push_back(f);
}

