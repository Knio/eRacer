from Core.Globals import *

class Starfield(Entity, eRacer.Starfield):
  def __init__(self, view, n, size):
    Entity.__init__(self)
    eRacer.Starfield.__init__(self, n, size)
    self.camera = view.camera
    view.AddRenderable(self)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    v = self.camera.GetViewMatrix()
    p = self.camera.GetPosition()
    self.Update(v, p)
    
