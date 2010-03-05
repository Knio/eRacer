from Core.Globals import *

class Marker(Entity):
  def __init__(self, scene, position=ORIGIN):
    Entity.__init__(self)
    self.pos  = position
    self.graphics = scene.CreateMovingMeshNode("Box")
    self.graphics.thisown = 0
  
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
      
    game().io.LoadMeshAsync(load, "leather-box.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    # print self.pos
    self.graphics.SetTransform(CreateMatrix(self.pos))

