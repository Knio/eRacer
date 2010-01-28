from Core.Globals import *

class Box(Entity):
  def __init__(self, game):
    Entity.__init__(self, game)

    self.box = eRacer.Box(True, 20, Vector3(0, 20, 0))
    self.graphics = game.graphics.scene.CreateMovingGeometry("Box")
    self.graphics.visible = False
    self.graphics.SetTransform(eRacer.CreateMatrix(Vector3(0, 0.5, 0)))
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game.io.LoadMeshAsync(load, self.graphics, "leather-box.x")   
    
  def Tick(self, time):
    Entity.Tick(self, time)
    #pos = self.box.GetPosition()
    #rot = self.box.GetOrientation()

    #matrix = eRacer.CreateMatrix(pos, rot)
    #self.graphics.SetTransform(matrix)
        
    self.graphics.SetTransform(self.box.GetTransform())
    
