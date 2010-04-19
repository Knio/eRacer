#include "Track.h"
#include "Core/Consts.h"
#include <float.h>
#include <stdexcept>
extern Constants CONSTS;

namespace Logic {

Frame Track::GetFrame(float d)
{
  // get the vector at arclength d along the b-spline
  
  // find the 'next' curve point
  while (d >= dist) d -= dist;
  while (d < 0)     d += dist;
  std::map<float, int>::iterator l = arclen.lower_bound(d);
    
  int n = track.size();
  
  int i2 = l->second;
  int i1 = (i2-1+n)%n;
  float d2 =                                           l->first;
  float d1 = (l==arclen.begin() ? --arclen.end() : --l) ->first;
  float alpha = (d-d1)/(d2-d1);
  
  Point3  pos = track[i1].position * (1-alpha)   + track[i2].position * alpha;
  Vector3 up  = track[i1].up       * (1-alpha)   + track[i2].up       * alpha;
  Vector3 fw  = track[i1].fw       * (1-alpha)   + track[i2].fw       * alpha;
  float dist  = track[i1].dist     * (1-alpha)   + track[i2].dist     * alpha;
  
  normalize(up);
  normalize(fw);

  return Frame(pos,up,fw,dist);
}


Frame Track::GetFrame(Point3 pos, float hint)
{
  int lap = (int)((hint+0.5)/dist);
  hint -= lap*dist;
  std::map<float, int>::iterator l = arclen.lower_bound(hint);
  int i2 = l->second;
  int n = track.size();
  if (hint == -1 || length(pos - track[i2].position) > 30)
  {
    float mint = FLT_MAX;
    for (int i=0;i<n;i+=16)
    {
      float t = length(pos - track[i].position);
      if (t < mint)
      {
        mint = t;
        i2 = i;
      }
    }
  }
  int r = 32;
  while (r)
  {
    float d1 = length(pos - track[(i2 - r + n)%n].position);
    float d2 = length(pos - track[(i2 + 0 + n)%n].position);
    float d3 = length(pos - track[(i2 + r + n)%n].position);
    
    if (d2 <= d3 && d2 <= d1)
      r /= 2;
    else if (d1 < d3)
      i2 -= r;
    else
      i2 += r;
  }
  Frame f = track[(i2+n)%n];
  f.dist += dist * (lap + i2/n);
  return f;
}



Point3 Track::GetPositionAt(float d){
	return GetFrame(d).position;
}

Vector3 Track::GetNormalAt(float d){
  return GetFrame(d).up;
}


void Track::Subdivide(int NSUBDIV)
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
            
            Vector3 fw = p2 - p1;
            normalize(fw);
            
            n1 = cross(fw, cross(n1, fw));
            n2 = cross(fw, cross(n2, fw));
            
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
    arclen[dist] = d-1;
}

void Track::CreateMesh(const vector<TrackVertex>& profile, std::vector<ID3DXMesh*>& outputMeshes, int framesPerMesh)
{
  assert(framesPerMesh>0);
  
  const unsigned int N = track.size();
  const unsigned int D = profile.size();
  
  
  // 100/10 = 10
  // 101/10 = 11  
  const unsigned int nMeshes = N/framesPerMesh + !!(N%framesPerMesh);
  const unsigned int frameStep = framesPerMesh;
  
  for(unsigned int meshIndex=0; meshIndex<nMeshes; meshIndex++)
  {
    //for the last mesh there may be less frames
    if(meshIndex==nMeshes-1 && (N%framesPerMesh))
      framesPerMesh = N % framesPerMesh;
    
    
    
    ID3DXMesh *mesh;
    
    HRESULT r = D3DXCreateMeshFVF(
      framesPerMesh*(D-1)*2,      // DWORD NumFaces,
      (framesPerMesh+1)*D,        // DWORD NumVertices,
      0,                          // DWORD Options,
      Vertex_Format,              // DWORD FVF,
      Graphics::GraphicsLayer::GetInstance().GetDevice(),         // LPDIRECT3DDEVICE9 pD3DDevice,
      &mesh                       // LPD3DXMESH * ppMesh
    );
    
    if(r==E_OUTOFMEMORY)
    {
      cout << "Ran out of memory creating meshes!" << endl;
      throw std::runtime_error("Failed to create mesh");
    }
    assert(SUCCEEDED(r));

    TrackVertex* meshverts;
    unsigned short* meshidx;
    assert(SUCCEEDED(mesh->LockVertexBuffer(0, (void**)&meshverts)));
    assert(SUCCEEDED(mesh->LockIndexBuffer(0,  (void**)&meshidx)));    


    
    for (int i=0; i<(framesPerMesh+1); i++){
      Frame frame = track[(meshIndex*frameStep+i)%N];
      Matrix tx = CreateMatrix(frame.position, frame.up, frame.fw);

      
      
      for(unsigned int j=0; j<D; j++)  
      {
        TrackVertex& v = meshverts[i*D + j];
        v.position = mul1(tx, profile[j].position);
        v.normal   = mul0(tx, profile[j].normal);
        v.tu = profile[j].tu;
        v.tv = profile[j].tv * frame.dist;
        
        if (profile[j].position.x > maxX)
          maxX = profile[j].position.x;
        
        if (profile[j].position.x < minX)
          minX = profile[j].position.x;
        
          
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
        
        if (i == framesPerMesh) continue;
        if (j == D-1) break;
        
        meshidx[((i*(D-1) + j)*2 + 0)*3 + 0] = (i+0)*D + ((j + 0)%D);
        meshidx[((i*(D-1) + j)*2 + 0)*3 + 1] = (i+0)*D + ((j + 1)%D);
        meshidx[((i*(D-1) + j)*2 + 0)*3 + 2] = (i+1)*D + ((j + 0)%D);
        
        meshidx[((i*(D-1) + j)*2 + 1)*3 + 0] = (i+0)*D + ((j + 1)%D);
        meshidx[((i*(D-1) + j)*2 + 1)*3 + 1] = (i+1)*D + ((j + 1)%D);
        meshidx[((i*(D-1) + j)*2 + 1)*3 + 2] = (i+1)*D + ((j + 0)%D);
      
      }
    }

    assert(SUCCEEDED(mesh->UnlockVertexBuffer()));
    assert(SUCCEEDED(mesh->UnlockIndexBuffer()));
    
    D3DXATTRIBUTERANGE att;
    att.AttribId     =  0;
    att.FaceStart    =  0;
    att.FaceCount    =  framesPerMesh*(D-1)*2;
    att.VertexStart  =  0;
    att.VertexCount  =  (framesPerMesh+1)*D;
    
    mesh->SetAttributeTable(&att, 1);

    outputMeshes.push_back(mesh);
  }
}

float Track::GetOffsetFromCentre(const Point3& pos){
	Frame frame = GetFrame(pos);
	Vector3 left = cross(frame.fw, frame.up);
	Vector3 vProj = project(pos-frame.position, left);
	float cosTheta = dot(vProj, left) / length(vProj);
	if(cosTheta > 0.98 && cosTheta < 1.02){
		// to the left
		return -length(vProj);
	}
	else{
		//to the right
		return length(vProj);
	}
}
}