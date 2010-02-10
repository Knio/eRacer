from Core.Globals import *
from Mapping      import Mapping, E

class GameMapping(Mapping):
  def KeyPressedEvent(self, key):
    if   key == KEY.W:      return E.PlayerAccelerateEvent  (+1.0)
    elif key == KEY.S:      return E.PlayerBrakeEvent       (True)
    elif key == KEY.A:      return E.PlayerTurnEvent        (-1.0)
    elif key == KEY.D:      return E.PlayerTurnEvent        (+1.0)
    elif key == KEY.UP:     return E.CameraAccelerateEvent  (+1.0)
    elif key == KEY.DOWN:   return E.CameraAccelerateEvent  (-1.0)
    elif key == KEY.LEFT:   return E.CameraStrafeEvent      (-1.0)
    elif key == KEY.RIGHT:  return E.CameraStrafeEvent      (+1.0)
    elif key == KEY.C:      return E.CameraChangedEvent     (    )
    elif key == KEY.ESCAPE: return E.PauseEvent             (    )
    elif key == KEY.R:      return E.ReloadConstsEvent      (    )
    elif key == KEY.SPACE:  return E.PlayJaguarSoundEvent   (    )


  def KeyReleasedEvent(self, key):
    return {
    KEY.W:     [E.PlayerAccelerateEvent  (0)],
    KEY.S:     [E.PlayerBrakeEvent(False)],
    KEY.A:     [E.PlayerTurnEvent        (0)],
    KEY.D:     [E.PlayerTurnEvent        (0)],
    KEY.UP:    [E.CameraAccelerateEvent  (-1.0)],
    KEY.DOWN:  [E.CameraAccelerateEvent  (+1.0)],
    KEY.LEFT:  [E.CameraStrafeEvent      (+1.0)],
    KEY.RIGHT: [E.CameraStrafeEvent      (-1.0)],
    }.get(key, None)
    
  def MouseMovedEvent(self, relX, relY):
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
    
  def GamepadStick1AbsoluteEvent(self, x, y):
    return E.PlayerTurnEvent(x/1000.0)

  def GamepadStick2AbsoluteEvent(self, relX, relY):
    #needs calibration
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
        
  def GamepadTriggerAbsoluteEvent(self, z):
    return E.PlayerAccelerateEvent(z/-1000.0)
    
  def GamepadButtonPressedEvent(self, button):
    if button == eRacer.BUTTON_START:   return E.PauseEvent()
    if button == eRacer.BUTTON_B:       return E.PlayerBrakeEvent(True)
    
  def GamepadButtonReleasedEvent(self, button):
    if button == eRacer.BUTTON_B:       return E.PlayerBrakeEvent(False)
