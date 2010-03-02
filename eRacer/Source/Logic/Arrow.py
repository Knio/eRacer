from Core.Globals import *

class Arrow(Entity):
  def __init__(self, scene, position=ORIGIN):
    Entity.__init__(self)
    self.position = position
    self.graphics = scene.CreateMovingMeshNode("Arrow")
  
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
      
    game().io.LoadMeshAsync(load, "arrow.x")
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.graphics.SetTransform(CreateMatrix(self.position, math.pi, X))
