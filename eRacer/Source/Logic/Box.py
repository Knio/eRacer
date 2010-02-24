from Core.Globals import *

class Box(Entity):
  def __init__(self, scene, position):
    Entity.__init__(self)
    self.transform = Matrix()
    self.position = position
    self.physics = eRacer.Box(False, 4000, position)
    self.graphics = scene.CreateMovingMeshNode("Box")
    self.graphics.thisown = 0
    self.graphics.SetTransform(self.transform)
  
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.initialized = True
      
    game().io.LoadMeshAsync(load, self.graphics, "leather-box.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()
    self.graphics.SetTransform(self.transform)  
