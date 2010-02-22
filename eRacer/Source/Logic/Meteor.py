from Core.Globals import *

class Meteor(Entity):
  def __init__(self, scene, position, force):
    Entity.__init__(self)
    self.position = position
    self.force = force
    self.physics = eRacer.Box(True, 4000, position)
    self.graphics = scene.CreateMovingMeshNode("Meteor")
    self.graphics.thisown = 0
    self.graphics.SetTransform(self.transform)
        
        
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      #self.graphics.initialized = True
      
    game().io.LoadMeshAsync(load, self.graphics, "leather-box.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()
    
  def transform_changed(self):
    self.graphics.SetTransform(self.transform)  
