from Game import Entity
import eRacer

class Box(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)

    self.box = eRacer.Box(True, 20, eRacer.D3DXVECTOR3(0, 20, 0))
    self.graphics = game.graphics.scene.CreateMovingGeometry("Box")
    self.graphics.visible = False
    self.graphics.SetTransform(eRacer.CreateMatrix(eRacer.D3DXVECTOR3(0, 0.5, 0)))
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game.io.LoadMeshAsync(load, self.graphics, "ship_06.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    pos = self.box.GetPosition()
    rot = self.box.GetOrientation()

    matrix = eRacer.CreateMatrix(pos, rot)
    self.graphics.SetTransform(matrix)
    
