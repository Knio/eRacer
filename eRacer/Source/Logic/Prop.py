from Core.Globals import *

class Prop(Entity):
  def __init__(self, scene, model='arrow.x', tx=IDENTITY):
    Entity.__init__(self)
    self.tx = tx
    self.graphics = scene.CreateMovingMeshNode(model) # static?
    
    eRacer.debug(tx)
    
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
      
    game().io.LoadMeshAsync(load, model)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.graphics.SetTransform(self.tx)
