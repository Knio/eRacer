from Core.Globals import *

class State(eRacer.State):
  MAPPING = None
  def __init__(self):
    self.parent   = None
    self.mapping  = self.MAPPING and self.MAPPING()
    self.active   = False
    
  def Tick(self, time):
    pass
    
  def Activate(self):
    self.active = True
    
  def Deactivate(self):
    self.active = False
    