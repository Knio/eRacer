from Core.Globals import *

class Starfield(Entity, eRacer.Starfield):
  def __init__(self, n, size):
    Entity.__init__(self)
    eRacer.Starfield.__init__(self, n, size)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.Update()
    
