from Core.Globals import *
from Mapping      import Mapping, E

class MenuMapping(Mapping):
  def KeyPressedEvent(self, key):
    if key == KEY.UP:     return E.MenuUpEvent()
    if key == KEY.DOWN:   return E.MenuDownEvent()
    if key == KEY.RETURN: return E.MenuSelectEvent()
    
class MainMenuMapping(MenuMapping):
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:     return E.QuitEvent()
    return MenuMapping.KeyPressedEvent(self, key)    

class PauseMenuMapping(MenuMapping):
  def KeyPressedEvent(self, key):
    if key == KEY.ESCAPE:     return E.UnPauseEvent()
    return MenuMapping.KeyPressedEvent(self, key)
    