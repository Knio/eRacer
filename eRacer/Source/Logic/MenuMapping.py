from Core.Globals import *
from Mapping      import Mapping, E

class MenuMapping(Mapping):
  def __init__(self):
    self.gamepadReleased = True
  
  def KeyPressedEvent(self, key):
    if key == KEY.UP:     return E.MenuUpEvent()
    if key == KEY.DOWN:   return E.MenuDownEvent()
    if key == KEY.RETURN: return E.MenuSelectEvent()
    
  def GamepadStick1AbsoluteEvent(self, x, y):
    if self.gamepadReleased:
      if y < -700.0:
        self.gamepadReleased = False
        return E.MenuUpEvent()
      elif y >  700.0:
        self.gamepadReleased = False
        return E.MenuDownEvent()
    elif abs(y) < 500:
      self.gamepadReleased = True
    
  def GamepadButtonPressedEvent(self, button):
    if button == cpp.BUTTON_A: return E.MenuSelectEvent()
    
class MainMenuMapping(MenuMapping):
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:     return E.QuitEvent()
    return MenuMapping.KeyPressedEvent(self, key)    

class PauseMenuMapping(MenuMapping):
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:     return E.UnPauseEvent()
    return MenuMapping.KeyPressedEvent(self, key)
    
  def GamepadButtonPressedEvent(self, button):
    if button == eRacer.BUTTON_START:  return E.UnPauseEvent()
    return MenuMapping.GamepadButtonPressedEvent(self, button)
    
