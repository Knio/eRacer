from Core.Globals import *

# There is now NO need for this to be an entity, and it shouldn't be one,
# except that Draw() is const and I don't want to change everything to non-const

class Starfield(Entity, eRacer.Starfield):
  def __init__(self, n, size):
    Entity.__init__(self)
    eRacer.Starfield.__init__(self, n, size)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.Update()
    
