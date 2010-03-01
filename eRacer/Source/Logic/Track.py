from Core.Globals import *

TRACK = [
   
  eRacer.Frame(Point3( 1000,   0,    00), Y),
  eRacer.Frame(Point3(-1000,   0,    00), Y),  
  eRacer.Frame(Point3(-1000,   5,  1000), normalized(Vector3( 1,0,-1))),
  eRacer.Frame(Point3(   00,  10,  1000), Y),
  eRacer.Frame(Point3(   00,  10, -1000), Y),  
  eRacer.Frame(Point3( 1000,   5, -1000), normalized(Vector3( 1,0,-1))),
  
]

PROFILE = [
  eRacer.TrackVertex(Point3(  25, -1,  0),  Y, 0.00, 0.01),
  eRacer.TrackVertex(Point3(  25,  1,  0),  Y, 0.00, 0.01),
  eRacer.TrackVertex(Point3(  25,  0,  0),  Y, 0.10, 0.01),
  eRacer.TrackVertex(Point3( -25,  0,  0),  Y, 0.90, 0.01),  
  eRacer.TrackVertex(Point3( -25,  1,  0),  Y, 1.00, 0.01),
  eRacer.TrackVertex(Point3( -25, -1,  0),  Y, 0.00, 0.01),  
  
  eRacer.TrackVertex(Point3(  25, -1,  0),  Y, 1.00, 0.01),
  
]

class Track(Entity):
  def __init__(self, scene):
    Entity.__init__(self)
    
    self.physics  = eRacer.TriMesh()
    self.graphics = scene.CreateStaticMeshNode("track")
    
    self.track = eRacer.Track()
    for i in TRACK:
      self.track.Add(i)
    
    self.track.Subdivide(6)
    
    profile = eRacer.VectorTrackVertex()
    for i in PROFILE:
      profile.push_back(i)
    
    mesh = self.track.CreateMesh(profile)
    tex  = game().io.LoadTexture('concrete_plates.jpg')
    mat  = game().graphics.graphics.DefaultMaterial()
    mesh.disown()
    mat.disown()
    self.mesh = eRacer.Mesh(mesh, mat, tex)
    
    self.graphics.Init(self.mesh)
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
    
  def GetPosition(self, dist):
    return self.track.Get(dist)
    
  def FindPosition(self, pos, hint=None):
    
    
  def Tick(self, time):
     Entity.Tick(self, time)

