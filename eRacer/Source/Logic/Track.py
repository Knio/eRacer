from Core.Globals import *

TRACK = [
   
  eRacer.Frame(Point3( 1000,   0,    00), Y),
  eRacer.Frame(Point3(-1000,   0,    00), Y),  
  eRacer.Frame(Point3(-1000,   5,  1000), normalized(Vector3( 1,0,-1))),
  eRacer.Frame(Point3(   00,  10,  1000), Y),
  eRacer.Frame(Point3(   00,  10, -1000), Y),  
  eRacer.Frame(Point3( 1000,   5, -1000), normalized(Vector3(-1,0, 1))),
  
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
    
    self.track.Subdivide(7)
    
    self.dist = self.track.GetTotalDist()
    
    profile = eRacer.VectorTrackVertex()
    for i in PROFILE:
      profile.push_back(i)
    
    mesh = self.track.CreateMesh(profile)
    tex  = game().io.LoadTexture('concrete_plates.jpg')
    mat  = game().graphics.graphics.DefaultMaterial()
    mesh.disown()
    self.mesh = eRacer.Mesh(mesh, mat, tex)
    
    self.graphics.Init(self.mesh)
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
    
  def GetFrame(self, dist):
    frame = self.track.Get(dist)
    print frame.position
    return frame
    
  def FindPosition(self, pos, hint=None):
    mind = 1e99
    if hint is None:
      for i in xrange(0, self.dist, self.dist/100.0):
        t = length(pos - self.track.Get(i).position)
        if t < mind:
          mind = t
          hint = i
    
    r = 100.0
    while r > 0.1:
      d1 = length(pos - self.track.Get(hint - r).position)
      d2 = length(pos - self.track.Get(hint    ).position)
      d3 = length(pos - self.track.Get(hint + r).position)
      
      if d2 <= d1 and d2 <= d3:
        r /= 2.0
        continue
        
      if d1 < d2:
        hint = hint - r
        
      else:
        hint = hint + r
      
    return hint
    
  def Tick(self, time):
     Entity.Tick(self, time)

