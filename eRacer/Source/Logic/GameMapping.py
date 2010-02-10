from Core.Globals import *
from Mapping      import Mapping, E

class GameMapping(Mapping):
  def KeyPressedEvent(self, key):
    return {
    KEY.W:      [E.PlayerAccelerateEvent  (+1.0)],
    KEY.S:      [E.PlayerBrakeEvent(True)],
    KEY.A:      [E.PlayerTurnEvent        (-1.0)],
    KEY.D:      [E.PlayerTurnEvent        (+1.0)],
    KEY.UP:     [E.CameraAccelerateEvent  (+1.0)],
    KEY.DOWN:   [E.CameraAccelerateEvent  (-1.0)],
    KEY.LEFT:   [E.CameraStrafeEvent      (-1.0)],
    KEY.RIGHT:  [E.CameraStrafeEvent      (+1.0)],
    KEY.C:      [E.CameraChangedEvent     (    )],
    KEY.ESCAPE: [E.PauseEvent             (    )],
    }.get(key, None)

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
    if button == eRacer.BUTTON_X:   return E.PauseEvent()
    if button == eRacer.BUTTON_B:       return E.PlayerBrakeEvent(True)
    
  def GamepadButtonReleasedEvent(self, button):
    if button == eRacer.BUTTON_B:       return E.PlayerBrakeEvent(False)
