from Core.Globals import *

class CoordinateCross(Entity, eRacer.CoordinateCross):
  def __init__(self, scene):
    Entity.__init__(self)
    eRacer.CoordinateCross.__init__(self)
    scene.AddRenderable(self)
    
  def Tick(self, time):
    Entity.Tick(self, time)