#include "Math.h"
#include <vector>
#include "Graphics/GraphicsLayer.h"
using namespace std;

// TODO namespace

struct Frame
{
  Point3  position;
  Vector3 up;
  Vector3 fw;
  float dist;
  Frame(const Point3& p, const Vector3& u, const Vector3& f=ORIGIN) : 
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
  
  void Add(const Frame &f)
  {
    track.push_back(f);
  }
  
  Frame Get(float d)
  {
    // get the vector at arclength d along the b-spline
    
    // find the 'next' curve point
    while (d >= dist) d -= dist;
    while (d < 0)     d += dist;
    std::map<float, int>::iterator l = arclen.lower_bound(d);
    
    assert(l!=arclen.end());
    
    int n = track.size();
    
    int i2 = l->second;
    int i1 = (i2-1+n)%n;
    float d2 =                                           l->first;
    float d1 = (l==arclen.begin() ? --arclen.end() : --l) ->first;
    float alpha = (d-d1)/(d2-d1);
    
    Point3  pos = track[i1].position * (1-alpha)   + track[i2].position * alpha;
    Vector3 up  = track[i1].up       * (1-alpha)   + track[i2].up       * alpha;
    Vector3 fw  = track[i1].fw       * (1-alpha)   + track[i2].fw       * alpha;
    
    normalize(up);
    normalize(fw);

    return Frame(pos, up, fw);
  }
  
  void Subdivide(int NSUBDIV=5)
  {
      
      vector<Frame> r1 = track;
      vector<Frame> r2;
      
      for (int C=0;C<NSUBDIV;C++)
      {
          int d = r1.size();
          for (int i=0;i<d;i++)
          {
              Point3 p1 = r1[i].position*(3/4.) + r1[(i-1+d)%d].position*(1/8.) + r1[(i+1)%d].position*(1/8.);
              Point3 p2 = r1[i].position*(2/4.) + r1[(i+1  )%d].position*(2/4.);

              Vector3 n1 = r1[i].up     *(3/4.) + r1[(i-1+d)%d].up      *(1/8.) + r1[(i+1)%d].up     *(1/8.);
              Vector3 n2 = r1[i].up     *(2/4.) + r1[(i+1  )%d].up      *(2/4.);
              
              normalize(n1);
              normalize(n2);
              
              r2.push_back(Frame(p1, n1));
              r2.push_back(Frame(p2, n2));
          }
          r1 = r2;
          r2.clear();
      }
      track = r1;
      
      // calculate arc-length parameterization and forward vector
      dist = 0;
      arclen.clear();
      int d = track.size();
      for (int i=0;i<d;i++)
      {
          arclen[dist] = i;
          Vector3 fw = track[(i+1)%d].position - track[i].position;
          track[i].fw = normalized(fw);
          track[i].dist = dist;
          dist += length(fw);
      }
      arclen[dist] = 0;
  }
  
  ID3DXMesh* CreateMesh(const vector<TrackVertex>& profile)
  {
    int N = track.size();
    int D = profile.size();
    
    // create vertex buffer
    ID3DXMesh *mesh;
    
    assert(SUCCEEDED(D3DXCreateMeshFVF(
      N*(D-1)*2,                  // DWORD NumFaces,
      N*D,                        // DWORD NumVertices,
      0, //D3DXMESH_USEHWONLY,         // DWORD Options,
      Vertex_Format,              // DWORD FVF,
      Graphics::GraphicsLayer::GetInstance()->GetDevice(),         // LPDIRECT3DDEVICE9 pD3DDevice,
      &mesh                       // LPD3DXMESH * ppMesh
    )));
    
    TrackVertex* meshverts;
    unsigned short* meshidx;
    assert(SUCCEEDED(mesh->LockVertexBuffer(0, (void**)&meshverts)));
    assert(SUCCEEDED(mesh->LockIndexBuffer(0,  (void**)&meshidx)));
    
    for (int i=0;i<N;i++)
    {      
      Frame frame = track[i];
      Vector3 &ap = frame.position;
      Vector3 &ay = frame.up;
      Vector3 &az = frame.fw;      
      Vector3  ax = cross(ay, az);
      /*
      
      http://msdn.microsoft.com/en-us/library/ee422511(VS.85).aspx

                            ( ax.x  ay.x  az.x    0 )
                            ( ax.y  ay.y  az.y    0 )
                            ( ax.z  ay.z  az.z    0 )
                          * ( ap.x  ap.y  ap.z    1 )      
      ( 1  0  0  0  )     = (                       )
      
            
      */
      Matrix tx(
         ax.x, ax.y, ax.z,   0,
         ay.x, ay.y, ay.z,   0,
         az.x, az.y, az.z,   0,
           
         ap.x, ap.y, ap.z,   1
      );
      // does the frame make sense?
      // assert(length(mul0(tx, X)      - ax) < 0.01);
      // assert(length(mul0(tx, Y)      - ay) < 0.01);
      // assert(length(mul0(tx, Z)      - az) < 0.01);
      // assert(length(mul1(tx, ORIGIN) - ap) < 0.01);
      
      for (int j=0;j<D;j++)  
      {
        TrackVertex& v = meshverts[i*D + j];
        v.position = mul1(tx, profile[j].position);
        v.normal   = mul0(tx, profile[j].normal);
        v.tu = profile[j].tu;
        v.tv = profile[j].tv * frame.dist;
        
        
        /* Index buffer
        
        Track looks like this:
        
        j
        0   1   2   3   4 ..
        
        |  \|  \|  \|  \|
        *---*---*---*---*-  i+1
        |\  |\  |\  |\  |\
        | \ | \ | \ | \ |
        |  \|  \|  \|  \|
        *---*---*---*---*- i+0
        |\  |\  |\  |\  |\
        
        
        vertex buffer:
        
        4--5
        |\ |
        | \|
        2--3
        |\ |
        | \|
        0--1
        
        idx buffer:
        
        0 1 2
        1 2 3
        2 3 4
        3 4 5
        
        */
        
        if (j == D-1) continue;
        
        meshidx[((i*(D-1) + j)*2 + 0)*3 + 0] = ((i+0)%N)*D + ((j + 0)%D);
        meshidx[((i*(D-1) + j)*2 + 0)*3 + 1] = ((i+0)%N)*D + ((j + 1)%D);
        meshidx[((i*(D-1) + j)*2 + 0)*3 + 2] = ((i+1)%N)*D + ((j + 0)%D);
        
        meshidx[((i*(D-1) + j)*2 + 1)*3 + 0] = ((i+0)%N)*D + ((j + 1)%D);
        meshidx[((i*(D-1) + j)*2 + 1)*3 + 1] = ((i+1)%N)*D + ((j + 1)%D);
        meshidx[((i*(D-1) + j)*2 + 1)*3 + 2] = ((i+1)%N)*D + ((j + 0)%D);
        
      }
    }
    
    assert(SUCCEEDED(mesh->UnlockVertexBuffer()));
    assert(SUCCEEDED(mesh->UnlockIndexBuffer()));
    
    D3DXATTRIBUTERANGE att;
    att.AttribId     =  0;
    att.FaceStart    =  0;
    att.FaceCount    =  N*(D-1)*2;
    att.VertexStart  =  0;
    att.VertexCount  =  N*D;
    
    mesh->SetAttributeTable(&att, 1);
    
    return mesh;
  }
};

