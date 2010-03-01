from Core.Globals import *

class Track(Entity):
  def __init__(self, scene):
    Entity.__init__(self)

    self.physics = eRacer.TriMesh()
    self.graphics = scene.CreateStaticMeshNode("track")
    # self.graphics.visible = False
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
        self.physics.Init(mesh.mesh())
      
    game().io.LoadMeshAsync(load, "Track1.x")
    self.physics.SetGroup(eRacer.TRACK)
    
    
  def Tick(self, time):
     Entity.Tick(self, time)
