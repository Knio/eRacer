from Core.Globals import *
from Mapping      import Mapping, E

class GameEndMapping(Mapping):
  def KeyPressedEvent(self, key):
    if   key == KEY.ESCAPE: return E.PauseEvent             (    )
    elif key == KEY.R:      return E.ReloadConstsEvent      (    )
    elif key == KEY.SPACE:  return E.PlayJaguarSoundEvent   (    )
    elif key == KEY.UP:     return E.CameraAccelerateEvent  (+1.0)
    elif key == KEY.DOWN:   return E.CameraAccelerateEvent  (-1.0)
    elif key == KEY.LEFT:   return E.CameraStrafeEvent      (-1.0)
    elif key == KEY.RIGHT:  return E.CameraStrafeEvent      (+1.0)
    elif key == KEY.C:      return E.CameraChangedEvent     (    )

  def KeyReleasedEvent(self, key):
    if   key == KEY.UP:     return E.CameraAccelerateEvent (-1.0)
    elif key == KEY.DOWN:   return E.CameraAccelerateEvent (+1.0)
    elif key == KEY.LEFT:   return E.CameraStrafeEvent     (+1.0)
    elif key == KEY.RIGHT:  return E.CameraStrafeEvent     (-1.0)
    
  def MouseMovedEvent(self, relX, relY):
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
        
  def GamepadButtonPressedEvent(self, button):
    if   button == eRacer.BUTTON_START:   return E.PauseEvent()
    elif button == eRacer.BUTTON_A:       return E.ReloadConstsEvent()
    elif button == eRacer.BUTTON_Y:       return E.CameraChangedEvent()
    elif button == eRacer.BUTTON_X:       CONSTS.CAR_DEBUG = not CONSTS.CAR_DEBUG
    
    