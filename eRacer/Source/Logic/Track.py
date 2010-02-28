from Core.Globals import *

TRACK = [
  eRacer.Frame(Point3( 1000, 0, 0), Y),
  eRacer.Frame(Point3(0, 0,  1000), Y),
  eRacer.Frame(Point3(-1000, 0, 0), Y),
  eRacer.Frame(Point3(0, 0, -1000), Y),
]

PROFILE = [
  eRacer.TrackVertex(Point3(), Point3(), 0.01, 0),
  eRacer.TrackVertex(Point3(), Point3(), 0.01, 0),
  eRacer.TrackVertex(Point3(), Point3(), 0.01, 0),
  eRacer.TrackVertex(Point3(), Point3(), 0.01, 0),
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
      
    self.track.Subdivide()
    
    
    mesh = self.track.CreateMesh(PROFILE)
    tex  = game().io.LoadTexture('concrete_plates.jpg')
    
    self.graphics.Init(mesh, 1, game().graphics.graphics.DefaultMaterial(), [tex])
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
    
  def Tick(self, time):
     Entity.Tick(self, time)
