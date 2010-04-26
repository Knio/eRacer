from Core.Globals import *
from Mapping      import Mapping, E

class MenuMapping(Mapping):
  def __init__(self):
    self.gamepadReleased = True
  
  def KeyPressedEvent(self, key):
    if   key == KEY.UP:     return E.MenuUpEvent()
    elif key == KEY.DOWN:   return E.MenuDownEvent()
    elif key == KEY.LEFT:   return E.MenuLeftEvent()
    elif key == KEY.RIGHT:  return E.MenuRightEvent()
    elif key == KEY.RETURN: return E.MenuSelectEvent()
    
  def GamepadStick1AbsoluteEvent(self, id, x, y):
    if self.gamepadReleased:
      if y < -700.0:
        self.gamepadReleased = False
        return E.MenuUpEvent()
      elif y >  700.0:
        self.gamepadReleased = False
        return E.MenuDownEvent()
      elif x < -700.0:
        self.gamepadReleased = False
        return E.MenuLeftEvent()
      elif x >  700.0:
        self.gamepadReleased = False
        return E.MenuRightEvent()
    elif abs(y) < 500 and abs(x) < 500:
      self.gamepadReleased = True
  def GamepadButtonPressedEvent(self, id, button):
    if button == cpp.BUTTON_A: return E.MenuSelectEvent()
    
class MainMenuMapping(MenuMapping):
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:     return E.QuitEvent()
    return MenuMapping.KeyPressedEvent(self, key) 

class PauseMenuMapping(MenuMapping):
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:     return E.UnPauseEvent()
    return MenuMapping.KeyPressedEvent(self, key)
    
  def GamepadButtonPressedEvent(self, id, button):
    if button == cpp.BUTTON_START:  return E.UnPauseEvent()
    elif button == cpp.BUTTON_A: return E.MenuSelectEvent()
    return MenuMapping.GamepadButtonPressedEvent(self, id, button)
    
