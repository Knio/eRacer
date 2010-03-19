from Core.Globals import *

class Model(Entity):
  def __init__(self, name, filename, phys=None, tx=IDENTITY, *args):
    Entity.__init__(self)
    self.transform = tx
    
    self.graphics = MeshNode(name+str(self.id),tx)
    self.physics  = phys
    
    if self.physics:
      self.physics.SetTransform(tx)
    
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
    game().io.LoadMeshAsync(load, filename, *args)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    if self.physics:
      self.transform = self.physics.GetTransform()
    self.graphics.SetTransform(self.transform)

