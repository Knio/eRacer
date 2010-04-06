#ifndef _TRACK_H
#define _TRACK_H
#include "Math.h"
#include <vector>
#include <iostream>
#include "Graphics/GraphicsLayer.h"
using namespace std;

/**
 * @brief a namespace to hold code related to game logic
 */
namespace Logic {
  
/**
 * @brief one coordinate frame 
 */
struct Frame
{
  Point3  position;
  Vector3 up;
  Vector3 fw;
  float dist;
  Frame(const Point3& p = ORIGIN, const Vector3& u=Y, const Vector3& f=X, float d=0.0) : 
    position(p), 
    up(u), 
    fw(f),
    dist(d)
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
   minX = 1;
   maxX = -1;
  }
  
  float GetTotalDist() { return dist; }
  
  void Add(const Frame &f);
  
  Frame GetFrame(float d);
  Frame GetFrame(Point3 pos, float hint=-1);
  
  Point3 GetPositionAt(float d);
  Vector3 GetNormalAt(float d);
  
  void Subdivide(int NSUBDIV=5);
  
  void CreateMesh(const vector<TrackVertex>& profile, std::vector<ID3DXMesh*>& outputMesh, int meshlen=1024);
  /**
  * returns distance from centre of track (where the x-coordinate of the profile is 0)
  * positive is on the right side of the track when looking down the positive z-axis
  */
  float GetOffsetFromCentre(const Point3& pos);
  float minX, maxX; //the width of the track
};

inline void Track::Add(const Frame &f)
{
   track.push_back(f);
}

}

#endif
