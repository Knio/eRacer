from Core.Globals import *

class Plane(Entity):
  def __init__(self, scene):
    Entity.__init__(self)

    self.physics  = eRacer.Plane(0, Vector3(0,1,0))
    self.graphics = scene.CreateMovingMeshNode("plane")
    self.graphics.thisown = 0
    
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.initialized = True
      
    game().io.LoadMeshAsync(load, self.graphics, "plane.x")   
    
  def Tick(self, time):
     Entity.Tick(self, time)
     pos = self.physics.GetPosition()
     self.graphics.SetTransform(self.physics.GetTransform())

