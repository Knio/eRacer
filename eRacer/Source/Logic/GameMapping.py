from Core.Globals import *
from Mapping      import Mapping, E

class GameMapping(Mapping):
  def KeyPressedEvent(self, key):
    if key == KEY.W: return E.PlayerAccelerateEvent(+1.0)
    if key == KEY.S: return E.PlayerAccelerateEvent(-1.0)
    if key == KEY.A: return E.PlayerTurnEvent      (-1.0)
    if key == KEY.D: return E.PlayerTurnEvent      (+1.0)
    
    if key == KEY.ESCAPE: return E.PauseEvent()
    
  def KeyReleasedEvent(self, key):
    return {
    KEY.W: [E.PlayerAccelerateEvent(-1.0)],
    KEY.S: [E.PlayerAccelerateEvent(+1.0)],
    KEY.A: [E.PlayerTurnEvent(+1.0)],
    KEY.D: [E.PlayerTurnEvent(-1.0)],
    }.get(key, None)