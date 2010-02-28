from Core.Globals import *

TRACK = [
  eRacer.Frame(Point3( 1000, 0,     0), Y),
  eRacer.Frame(Point3(    0, 0,  1000), Y),
  eRacer.Frame(Point3(-1000, 0,     0), Y),
  eRacer.Frame(Point3(    0, 0, -1000), Y),
]

PROFILE = [
  eRacer.TrackVertex(Point3( 10, -1, 0), -Y, 0.01, 0.00),
  eRacer.TrackVertex(Point3( 10,  0, 0),  Y, 0.01, 0.10),
  eRacer.TrackVertex(Point3(-10,  0, 0),  Y, 0.01, 0.90),
  eRacer.TrackVertex(Point3(-10, -1, 0), -Y, 0.01, 1.00),
]

PROFILE = [
  eRacer.TrackVertex(Point3(  0,  0,  0),  Y, 0.01, 0.00),
  eRacer.TrackVertex(Point3(  0, 10,  0),  Y, 0.01, 1.00),
]





class Track(Entity):
  def __init__(self, scene):
    Entity.__init__(self)

    self.physics  = eRacer.TriMesh()
    self.graphics = scene.CreateStaticMeshNode("track")
    # self.graphics.visible = False
        
    self.track = eRacer.Track()
    for i in TRACK:
      self.track.Add(i)
      
    self.track.Subdivide(4)
    
    profile = eRacer.VectorTrackVertex()
    for i in PROFILE:
      profile.push_back(i)
    
    mesh = self.track.CreateMesh(profile)
    tex  = game().io.LoadTexture('concrete_plates.jpg')
    mat  = game().graphics.graphics.DefaultMaterial()
    mesh.disown()
    mat.disown()
    
    self.graphics.Init(mesh, mat, tex)
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
    
  def Tick(self, time):
     Entity.Tick(self, time)
