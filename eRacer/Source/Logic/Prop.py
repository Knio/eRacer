from Core.Globals import *

class Prop(Entity):
  def __init__(self, gfx, model, phys=None, tx=IDENTITY, *args):
    Entity.__init__(self)
    self.transform = tx
    
    self.graphics = gfx
    self.physics  = phys
    
    if self.physics:
      self.physics.SetTransform(tx)
    
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
    game().io.LoadMeshAsync(load, model, *args)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    if self.physics:
      self.transform = self.physics.GetTransform()
    self.graphics.SetTransform(self.transform)

