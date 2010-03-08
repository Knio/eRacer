from Core.Globals import *

TRACK = [	
  eRacer.Frame(Point3( -125,   0,   0), +Y),
  eRacer.Frame(Point3( -375,   0,   0), -Z),
  eRacer.Frame(Point3( -625,   0,   0), -Y),
  eRacer.Frame(Point3( -875,   0,   0), +Z),
  eRacer.Frame(Point3(-1125,   -75,   0), +Y),
  eRacer.Frame(Point3(-1375,   -75,   0), +X+Y+Z),
  eRacer.Frame(Point3(-1375,-125, 500), +X+Y-Z),
  eRacer.Frame(Point3( -750,-250, 500), -X+Y-Z),
  
  eRacer.Frame(Point3(-1000,-500,   0), +Y),
  eRacer.Frame(Point3( -750,   0,-500), +Z),
  eRacer.Frame(Point3( -750, 500,   0), -Y),
  eRacer.Frame(Point3( -750,   0, 500), -Z),
  eRacer.Frame(Point3( -500,-500,   0), +Y),
  
  eRacer.Frame(Point3( -375,-250, -550), -X+Y+Z),
  eRacer.Frame(Point3(-1125,-125,-1125), +X+Y+Z),

  eRacer.Frame(Point3(-1125,  50,  -50), -Y),
  eRacer.Frame(Point3( -875,  50,  -50), -Z),
  eRacer.Frame(Point3( -625,  50,  -50), +Y),
  eRacer.Frame(Point3( -375,  50,  -50), +Z),
  eRacer.Frame(Point3( -125,  50,  -50), -Y),
  eRacer.Frame(Point3(  125,  50,  -50), -Y),
  eRacer.Frame(Point3(  375,  50,  -50), -Z),
  eRacer.Frame(Point3(  625,  50,  -50), +Y),
  eRacer.Frame(Point3(  875,  50,  -50), +Z),
  eRacer.Frame(Point3( 1125,  50,  -50), -Y),
  
  
  eRacer.Frame(Point3( 1375,  25,  -50), -X-Y-Z),
  eRacer.Frame(Point3( 1375,-300, -550), -X+Z),
  eRacer.Frame(Point3(  375,-300, -550), +X+Y+Z),
  
  eRacer.Frame(Point3(  375, 675, 550), +X+Y-Z),
  eRacer.Frame(Point3( 1375, 675, 550), -X+Y-Z),
  eRacer.Frame(Point3( 1375, 600,   0), -X+Y+Z),
  
  eRacer.Frame(Point3(  375, 525, 175), +X+Y+Z),
  eRacer.Frame(Point3(  375, 450, 550), +X+Y-Z),
  eRacer.Frame(Point3( 1375, 375, 550), -X+Y-Z),
  eRacer.Frame(Point3( 1375, 300,   0), -X+Y+Z),
  
  eRacer.Frame(Point3(  375, 225,  50), +X+Y+Z),
  eRacer.Frame(Point3(  375, 150, 550), +X+Y-Z),
  eRacer.Frame(Point3( 1375,  75, 550), -X+Y-Z),
  eRacer.Frame(Point3( 1375,   0,   0), -X+Y+Z),
  
  eRacer.Frame(Point3( 1125,   0,   0), +Y),
  eRacer.Frame(Point3(  875,   0,   0), -Z),
  eRacer.Frame(Point3(  625,   0,   0), -Y),
  eRacer.Frame(Point3(  375,   0,   0), +Z),
  eRacer.Frame(Point3(  125,   0,   0), +Y),
]

PROFILE = [
  eRacer.TrackVertex(Point3(  25, -1,  0),  Y, 0.00, 0.01),
  eRacer.TrackVertex(Point3(  25,  2,  0),  Y, 0.00, 0.01),
  eRacer.TrackVertex(Point3(  25,  0,  0),  Y, 0.05, 0.01),
  eRacer.TrackVertex(Point3(  15,  0,  0),  Y, 0.20, 0.01),  
  eRacer.TrackVertex(Point3(   5,  0,  0),  Y, 0.40, 0.01),  
  eRacer.TrackVertex(Point3(  -5,  0,  0),  Y, 0.60, 0.01),  
  eRacer.TrackVertex(Point3( -15,  0,  0),  Y, 0.80, 0.01),  
  eRacer.TrackVertex(Point3( -25,  0,  0),  Y, 0.95, 0.01),  
  eRacer.TrackVertex(Point3( -25,  2,  0),  Y, 1.00, 0.01),
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
    mat.disown()
    self.mesh = eRacer.Mesh(mesh, mat, tex)
    
    self.graphics.Init(self.mesh)
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
  # def FindPositionFull(self, pos):
  #   mind = 1e99
  #   hint = 0
  #   for i in xrange(0, self.dist, self.dist/500.0):
  #     frame = self.GetFrame(i)
  #     t = length(pos - frame.position)
      
  #     if t < mind:
  #       mind = t
  #       hint = i
  #   return hint
    
  # def FindPosition(self, pos, hint=None):
  #   if not hint or length(pos - self.GetPositionAt(hint)) > 25:
  #     lap = hint and self.dist * int(hint/self.dist) or 0.
  #     hint = self.FindPositionFull(pos) + lap
    
  #   r = 100.0
  #   while r > 0.1:
  #     d1 = length(pos - self.GetPositionAt(hint - r))
  #     d2 = length(pos - self.GetPositionAt(hint    ))
  #     d3 = length(pos - self.GetPositionAt(hint + r))
      
  #     if d2 <= d1 and d2 <= d3:
  #       r /= 2.0
  #       continue
        
  #     if d1 < d2:
  #       hint = hint - r
        
  #     else:
  #       hint = hint + r
    
  #   return hint
    
  def Tick(self, time):
     Entity.Tick(self, time)

     

