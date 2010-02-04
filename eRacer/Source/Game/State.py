from Core.Globals import *

class State(eRacer.State):
  MAPPING = None
  def __init__(self):
    self.parent   = None
    self.mapping  = self.MAPPING and self.MAPPING()
    
  def Tick(self, time):
    pass
    
  def Activate(self):
    pass
    
  def Deactivate(self):
    pass
    