from Core.Globals import *

class Ship(Entity):
  def __init__(self, scene):
    Entity.__init__(self)
    
    # no physics yet..
    self.graphics = scene.CreateMovingGeometry("test")
    self.graphics.thisown = 0
    self.transform = eRacer.CreateMatrix(Vector3(10,3,4))
    self.graphics.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game().io.LoadMeshAsync(load, self.graphics, "Ship_06.x")
    
  def Tick(self, time):
    pass
  
  def transform_changed(self):
    self.graphics.SetTransform(self.transform)  