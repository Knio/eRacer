from Core.Globals import *

class StaticModel(Entity):
  def __init__(self, name, filename, tx=IDENTITY, *args):
    Entity.__init__(self)
    self.transform = tx
    
    self.name = name+str(self.id)
    self.graphics = MeshNode(self.name, tx)
    
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)
    game().io.LoadMeshAsync(load, filename, *args)
    
    
  def Release(self):
    self.graphics.Release()
    if self.physics:
      self.physics.Release()
      
      
class Model(StaticModel):
  def __init__(self, name, filename, phys=None, tx=IDENTITY, *args):
    StaticModel.__init__(self, name, filename, tx, *args)
    self.physics  = phys
    
    if self.physics:
      self.physics.SetTransform(tx)
      
  
  def Tick(self, time):
    Entity.Tick(self, time)
    if self.physics:
      self.transform = self.physics.GetTransform()
    self.graphics.SetTransform(self.transform)
  