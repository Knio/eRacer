from Core.Globals import *

class Ship(Entity):
  def __init__(self, scene):
    Entity.__init__(self)
    
    # no physics yet..
    self.graphics = scene.CreateMovingMeshNode("Old Ship Model")
    self.graphics.thisown = 0
    self.transform = eRacer.CreateMatrix(Vector3(10,3,4))
        
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
      
    game().io.LoadMeshAsync(load, "Ship1.x")
    
  def Tick(self, time):
    pass
  
  def transform_changed(self):
    self.graphics.SetTransform(self.transform)  