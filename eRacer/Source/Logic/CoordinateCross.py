from Core.Globals import *

class CoordinateCross(Entity, eRacer.CoordinateCross):
  def __init__(self, view):
    Entity.__init__(self)
    eRacer.CoordinateCross.__init__(self)
    view.AddRenderable(self)
    
  def Tick(self, time):
    Entity.Tick(self, time)