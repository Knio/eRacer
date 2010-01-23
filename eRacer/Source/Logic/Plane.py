from Game import Entity
from Main import Main
import eRacer

class Plane(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)

    self.physics = eRacer.Plane(False, 0, 0, eRacer.D3DXVECTOR3(0,1,0))
    self.graphics = game.graphics.scene.CreateMovingGeometry("plane")
    self.graphics.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game.io.LoadMeshAsync(load, self.graphics, "plane.x")   
    
  def Tick(self, time):
     Entity.Tick(self, time)
     pos = self.physics.GetPosition()
     #print pos.x , pos.y, pos.z
