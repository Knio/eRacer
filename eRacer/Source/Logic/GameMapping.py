from Core.Globals import *
from Mapping      import Mapping, E

class GameMapping(Mapping):
  def KeyPressedEvent(self, key):
    return {
    KEY.W:      [E.PlayerAccelerateEvent  (+1.0)],
    KEY.S:      [E.PlayerAccelerateEvent  (-1.0)],
    KEY.A:      [E.PlayerTurnEvent        (-1.0)],
    KEY.D:      [E.PlayerTurnEvent        (+1.0)],
    KEY.UP:     [E.CameraAccelerateEvent  (+1.0)],
    KEY.DOWN:   [E.CameraAccelerateEvent  (-1.0)],
    KEY.LEFT:   [E.CameraStrafeEvent      (-1.0)],
    KEY.RIGHT:  [E.CameraStrafeEvent      (+1.0)],
    KEY.ESCAPE: [E.PauseEvent             (    )],
    }.get(key, None)

  def KeyReleasedEvent(self, key):
    return {
    KEY.W:     [E.PlayerAccelerateEvent  (-1.0)],
    KEY.S:     [E.PlayerAccelerateEvent  (+1.0)],
    KEY.A:     [E.PlayerTurnEvent        (+1.0)],
    KEY.D:     [E.PlayerTurnEvent        (-1.0)],
    KEY.UP:    [E.CameraAccelerateEvent  (-1.0)],
    KEY.DOWN:  [E.CameraAccelerateEvent  (+1.0)],
    KEY.LEFT:  [E.CameraStrafeEvent      (+1.0)],
    KEY.RIGHT: [E.CameraStrafeEvent      (-1.0)],
    }.get(key, None)
    
  def MouseMovedEvent(self, relX, relY):
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
    
  def GamepadStick1RelativeEvent(self, x, y):
    return E.PlayerTurnEvent(x/1000.0)

  def GamepadStick2RelativeEvent(self, x, y):
    #needs calibration
    return E.CameraLookAroundEvent(relX/300.,relY/300.)

    
  def GamepadTriggerRelativeEvent(self, z):
    return E.PlayerAccelerateEvent(z/-1000.0)
    
  def GamepadButtonPressedEvent(self, button):
    return E.PauseEvent()
    
