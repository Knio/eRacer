from Game import Entity
from Main import Main
import eRacer

class Box(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)

    self.box = eRacer.Box(True, 20)
    self.graphics = game.graphics.scene.CreateMovingGeometry("Box")
    self.graphics.visible = False
    self.graphics.SetTransform(eRacer.CreateMatrix(eRacer.D3DXVECTOR3(0, 10, 0)))
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      print 'Loaded mesh'
      self.graphics.visible = True
      
    game.io.LoadMeshAsync(load, self.graphics, "box.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    pos = self.box.GetPosition()
    rot = self.box.GetOrientation()
    
    eRacer.debug(rot)
    print
    
    matrix = eRacer.CreateMatrix(pos, rot)
    
    eRacer.debug(matrix)
    print
    
    self.graphics.SetTransform(matrix)
    
