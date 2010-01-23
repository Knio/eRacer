from Game import Entity
from Main import Main
import eRacer

class Ship(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)
    
    # no physics yet..
    self.graphics = game.graphics.scene.CreateMovingGeometry("test")
    self.graphics.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game.io.LoadMeshAsync(load, self.graphics, "Ship_06.x")   
    
  def Tick(self, time):
    pass
