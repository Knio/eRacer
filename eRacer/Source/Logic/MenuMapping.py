import time

from Core.Globals import *
from Mapping      import Mapping, E

class MenuMapping(Mapping):
  MIN_INTERVAL = .5
  
  def __init__(self):
    self.lastMenuTime = time.time()

  
  def KeyPressedEvent(self, key):
    if key == KEY.UP:     return E.MenuUpEvent()
    if key == KEY.DOWN:   return E.MenuDownEvent()
    if key == KEY.RETURN: return E.MenuSelectEvent()
    
  def GamepadStick1AbsoluteEvent(self, x, y):
    if y >  900.0 and (time.time()-self.lastMenuTime) > self.MIN_INTERVAL:
      self.lastMenuTime = time.time()
      return E.MenuUpEvent()
    if y < -900.0 and (time.time()-self.lastMenuTime) > self.MIN_INTERVAL:
      self.lastMenuTime = time.time()
      return E.MenuDownEvent()
    
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
    
