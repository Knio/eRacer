from Game import Entity
from Main import Main
import eRacer

class Box(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)

    self.box = eRacer.Box(True, 20, eRacer.D3DXVECTOR3(0, 20, 0))
    self.graphics = game.graphics.scene.CreateMovingGeometry("test")
    self.graphics.visible = False
    self.graphics.SetTransform(eRacer.CreateMatrix(eRacer.D3DXVECTOR3(0, 0.5, 0)))
    eRacer.debug(self.graphics.GetTransform())
        
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
    
    #eRacer.debug(rot)
    
    matrix = eRacer.CreateMatrix(pos)
    
    #eRacer.debug(matrix)
    
    #self.graphics.SetTransform(matrix)
    
