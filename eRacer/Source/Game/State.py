from Core.Globals import *

class State(eRacer.State):
  def __init__(self):
    self.parent = None
    
  def Tick(self, time):
    pass
    
  def KeyPressedEvent(self, key): 
    if key == KEY.ESCAPE:
      game().event.QuitEvent()

  def Activate(self):
    pass
    
  def Deactivate(self):
    pass
    
