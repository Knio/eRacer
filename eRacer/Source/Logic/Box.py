from Core.Globals import *

class Box(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)

    self.physics = eRacer.Box(True, 20, Vector3(0, 20, 0))
    self.graphics = game.graphics.scene.CreateMovingGeometry("Box")
    self.graphics.visible = False
    self.graphics.SetTransform(self.transform)
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game.io.LoadMeshAsync(load, self.graphics, "leather-box.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()
    
  def set_transform(self, transform):
    Entity.set_transform(self, transform)
    self.graphics.SetTransform(self.transform)  

  transform = property(Entity.get_transform, set_transform)    
