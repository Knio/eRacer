from Core.Globals import *

TRACK = [
   
  eRacer.Frame(Point3(  500,   0,   00), Y),
  eRacer.Frame(Point3( -500,   0,   00), Y),  
  eRacer.Frame(Point3( -500,  25,  500), normalized(Vector3( 1,0,-1))),
  eRacer.Frame(Point3(   00,  50,  500), -Y),
  
  eRacer.Frame(Point3(  -37,  50, -250), -Y),
  eRacer.Frame(Point3(   00,-350, -200), -X),
  eRacer.Frame(Point3(   00,-350, +200), +Y-Z),
  eRacer.Frame(Point3(  +37,  50, +250), -Y),
  
  eRacer.Frame(Point3(   00,  50, -500), -Y),  
  eRacer.Frame(Point3(  500,  25, -500), normalized(Vector3(-1,0, 1))),
  
]

PROFILE = [
  eRacer.TrackVertex(Point3(  25, -1,  0),  Y, 0.00, 0.01),
  eRacer.TrackVertex(Point3(  25,  1,  0),  Y, 0.00, 0.01),
  eRacer.TrackVertex(Point3(  25,  0,  0),  Y, 0.05, 0.01),
  eRacer.TrackVertex(Point3(  15,  0,  0),  Y, 0.20, 0.01),  
  eRacer.TrackVertex(Point3(   5,  0,  0),  Y, 0.40, 0.01),  
  eRacer.TrackVertex(Point3(  -5,  0,  0),  Y, 0.60, 0.01),  
  eRacer.TrackVertex(Point3( -15,  0,  0),  Y, 0.80, 0.01),  
  eRacer.TrackVertex(Point3( -25,  0,  0),  Y, 0.95, 0.01),  
  eRacer.TrackVertex(Point3( -25,  1,  0),  Y, 1.00, 0.01),
  eRacer.TrackVertex(Point3( -25, -1,  0),  Y, 0.00, 0.01),
  
  eRacer.TrackVertex(Point3(  25, -1,  0),  Y, 1.00, 0.01),
  
]

class Track(Entity, eRacer.Track):
  def __init__(self, scene):
    Entity.__init__(self)
    eRacer.Track.__init__(self)
    
    self.physics  = eRacer.TriMesh()
    self.graphics = scene.CreateStaticMeshNode("track")
    
    for i in TRACK:
      self.Add(i)
    
    self.Subdivide(6)
    
    self.dist = self.GetTotalDist()
    
    profile = eRacer.VectorTrackVertex()
    for i in PROFILE:
      profile.push_back(i)
    
    mesh = self.CreateMesh(profile)
    tex  = game().io.LoadTexture('concrete_plates.jpg')
    mat  = game().graphics.graphics.DefaultMaterial()
    mesh.disown()
    self.mesh = eRacer.Mesh(mesh, mat, tex)
    
    self.graphics.Init(self.mesh)
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
  def FindPosition(self, pos, hint=None):
    mind = 1e99
    if hint is None:
      for i in xrange(0, self.dist, self.dist/500.0):
        frame = self.GetFrame(i)
        t = length(pos - frame.position)
        
        if t < mind:
          mind = t
          hint = i
    
    r = 100.0
    while r > 0.1:
      d1 = length(pos - self.GetPositionAt(hint - r))
      d2 = length(pos - self.GetPositionAt(hint    ))
      d3 = length(pos - self.GetPositionAt(hint + r))
      
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

     

