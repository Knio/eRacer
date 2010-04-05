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

  def KeyReleasedEvent(self, key):
    if   key == KEY.UP:     return E.CameraAccelerateEvent (-1.0)
    elif key == KEY.DOWN:   return E.CameraAccelerateEvent (+1.0)
    elif key == KEY.LEFT:   return E.CameraStrafeEvent     (+1.0)
    elif key == KEY.RIGHT:  return E.CameraStrafeEvent     (-1.0)
    
  def MouseMovedEvent(self, relX, relY):
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
        
  def GamepadButtonPressedEvent(self, gamepadId, button):
    if   button == cpp.BUTTON_START:   return E.PauseEvent()
    elif button == cpp.BUTTON_A:       return E.ReloadConstsEvent()
    elif button == cpp.BUTTON_Y:       return E.CameraChangedEvent()
    elif button == cpp.BUTTON_X:       CONSTS.CAR_DEBUG = not CONSTS.CAR_DEBUG
    
    