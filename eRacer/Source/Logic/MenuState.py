from Core.Globals import *
from Game.State   import State

from Camera     import CirclingCamera

class MenuState(State):
  def __init__(self):
    State.__init__(self)
    self.scene  = eRacer.Scene()
    self.camera = CirclingCamera()
    
  def Tick(self, time):
    State.Tick(self, time)
    game().graphics.scene  = self.scene
    game().graphics.camera = self.camera
  
  def KeyPressedEvent(self, key):
    pass

class MainMenuState(MenuState):
  def __init__(self):
    MenuState.__init__(self)
    
  def KeyPressedEvent(self, key):
    if key == KEY.RETURN:
      game().PushState(GameState())
    else:
      MenuState.KeyPressedEvent(self, key)      
    
  def Tick(self, time):
    game().graphics.graphics.WriteString(
      "Press ENTER to begin a new game", 
      "Verdana", 40, Point3(100,100,0)
    )
    MenuState.Tick(self, time)    
    

class PauseMenuState(MenuState):
  def __init__(self):
    MenuState.__init__(self)
    
  def Activate(self):
    game().simspeed = 0.
  
  def Deactivate(self):
    game().simspeed = 1.
    
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:
      game().PopState()
    else:
      MenuState.KeyPressedEvent(self, key)
    
  def Tick(self, time):
    game().graphics.graphics.WriteString(
      "PAUSED",
      "Verdana", 40, Point3(300,100,0)
    )
    MenuState.Tick(self, time)
    self.parent.Tick(time)

from GameState  import GameState
