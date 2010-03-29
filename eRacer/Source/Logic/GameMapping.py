from Core.Globals import *
from Mapping      import Mapping, E

class VehicleMapping(Mapping):
    def __init__(self,target):
        self.target = target
        
class Keyboard1Mapping(VehicleMapping):
  def __init__(self,target):
        VehicleMapping.__init__(self,target)
  
  def KeyPressedEvent(self, key):
    if   key == KEY.W:      return self.target.vehicle.behavior.PlayerAccelerateEvent  (+1.0)
    elif key == KEY.S:      return self.target.vehicle.behavior.PlayerAccelerateEvent  (-1.0)
    elif key == KEY.LSHIFT: return self.target.vehicle.behavior.PlayerBrakeEvent       (True)
    elif key == KEY.A:      return self.target.vehicle.behavior.PlayerTurnEvent        (-1.0)
    elif key == KEY.D:      return self.target.vehicle.behavior.PlayerTurnEvent        (+1.0)
    elif key == KEY.SPACE:  return self.target.vehicle.behavior.PlayerBoostEvent       (True)
    elif key == KEY.C:      return self.target.CameraChangedEvent               (    )
    elif key == KEY.ESCAPE: return E.PauseEvent                       (    )
    elif key == KEY.T:      return self.target.vehicle.behavior.RespawnCarEvent        (    )
    elif key == KEY.F5:     return self.target.DebugCameraToggle()
    
  def KeyReleasedEvent(self, key):
    if   key == KEY.W:     return self.target.vehicle.behavior.PlayerAccelerateEvent ( 0)
    elif key == KEY.S:     return self.target.vehicle.behavior.PlayerAccelerateEvent ( 0)
    elif key == KEY.LSHIFT:return self.target.vehicle.behavior.PlayerBrakeEvent      (False)
    elif key == KEY.A:     return self.target.vehicle.behavior.PlayerTurnEvent       ( 0)
    elif key == KEY.D:     return self.target.vehicle.behavior.PlayerTurnEvent       ( 0)
    elif key == KEY.SPACE: return self.target.vehicle.behavior.PlayerBoostEvent     (False)
        
class Keyboard2Mapping(VehicleMapping):
  def __init__(self,target):
    VehicleMapping.__init__(self,target)
        
  def KeyPressedEvent(self, key):
    if   key == KEY.UP:         return self.target.vehicle.behavior.PlayerAccelerateEvent  (+1.0)
    elif key == KEY.DOWN:       return self.target.vehicle.behavior.PlayerAccelerateEvent  (-1.0)
    elif key == KEY.RCONTROL:   return self.target.vehicle.behavior.PlayerBrakeEvent       (True)
    elif key == KEY.LEFT:       return self.target.vehicle.behavior.PlayerTurnEvent        (-1.0)
    elif key == KEY.RIGHT:      return self.target.vehicle.behavior.PlayerTurnEvent        (+1.0)
    elif key == KEY.NUMPAD0:    return self.target.vehicle.behavior.PlayerBoostEvent       (True)
    elif key == KEY.DECIMAL:    return self.target.CameraChangedEvent               (    )
    elif key == KEY.PAUSE:      return E.PauseEvent                       (    )
    elif key == KEY.RSHIFT:     return self.target.vehicle.behavior.RespawnCarEvent        (    )
    
    
  def KeyReleasedEvent(self, key):
    if   key == KEY.UP:         return self.target.vehicle.behavior.PlayerAccelerateEvent ( 0)
    elif key == KEY.DOWN:       return self.target.vehicle.behavior.PlayerAccelerateEvent ( 0)
    elif key == KEY.RCONTROL:   return self.target.vehicle.behavior.PlayerBrakeEvent      (False)
    elif key == KEY.LEFT:       return self.target.vehicle.behavior.PlayerTurnEvent       ( 0)
    elif key == KEY.RIGHT:      return self.target.vehicle.behavior.PlayerTurnEvent       ( 0)
    elif key == KEY.NUMPAD0:    return self.target.vehicle.behavior.PlayerBoostEvent     (False)

class KeyboardDebugMapping(VehicleMapping):
  def __init__(self,target):
    VehicleMapping.__init__(self,target)
    
  def KeyPressedEvent(self, key):
    if key == KEY.UP:       return E.CameraAccelerateEvent  (+1.0)
    elif key == KEY.DOWN:   return E.CameraAccelerateEvent  (-1.0)
    elif key == KEY.LEFT:   return E.CameraStrafeEvent      (-1.0)
    elif key == KEY.RIGHT:  return E.CameraStrafeEvent      (+1.0)
    elif key == KEY.R:      return E.ReloadConstsEvent      (    )  

  def KeyReleasedEvent(self, key):
    if   key == KEY.UP:    return E.CameraAccelerateEvent (-1.0)
    elif key == KEY.DOWN:  return E.CameraAccelerateEvent (+1.0)
    elif key == KEY.LEFT:  return E.CameraStrafeEvent     (+1.0)
    elif key == KEY.RIGHT: return E.CameraStrafeEvent     (-1.0)
        
    elif key == KEY.TAB:  
      if CONSTS.CAR_DEBUG: 
        print "DebugMode is On"
        CONSTS.CAR_DEBUG = not CONSTS.CAR_DEBUG
      else: 
        print "DebugMode is OFf"
        return E.ReloadConstsEvent()

  def MouseMovedEvent(self, relX, relY):
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
      

class GamepadMapping(VehicleMapping):
  def __init__(self, gamepadId, target):
    VehicleMapping.__init__(self,target)
    self.gamepadId = gamepadId

  def GamepadStick1AbsoluteEvent(self, gamepadId, x, y):
    if gamepadId == self.gamepadId:
      return self.target.vehicle.behavior.PlayerTurnEvent(x/1000.0)  

  def GamepadTriggerAbsoluteEvent(self, gamepadId, z):
    if gamepadId == self.gamepadId:
      return self.target.vehicle.behavior.PlayerAccelerateEvent(z/-1000.0)
    
  def GamepadButtonPressedEvent(self, gamepadId, button):
    if gamepadId == self.gamepadId:
      if   button == cpp.BUTTON_START:
        return E.PauseEvent()
      elif button == cpp.BUTTON_A:
        return self.target.vehicle.behavior.PlayerBoostEvent(True)
      elif button == cpp.BUTTON_B:
        return self.target.vehicle.behavior.PlayerBrakeEvent(True)
      elif button == cpp.BUTTON_Y:
        return self.target.CameraChangedEvent()

  def GamepadButtonReleasedEvent(self, gamepadId, button):
    if gamepadId == self.gamepadId:
      if button == cpp.BUTTON_B:
        return self.target.vehicle.behavior.PlayerBrakeEvent(False)
      elif button == cpp.BUTTON_A:
        return self.target.vehicle.behavior.PlayerBoostEvent(False)
        
class Gamepad1Mapping(GamepadMapping):
  def __init__(self, target):
    GamepadMapping.__init__(self, 0, target)

class Gamepad2Mapping(GamepadMapping):
  def __init__(self, target):
    GamepadMapping.__init__(self, 1, target)

class Gamepad3Mapping(GamepadMapping):
  def __init__(self, target):
    GamepadMapping.__init__(self, 2, target)

class Gamepad4Mapping(GamepadMapping):
  def __init__(self, target):
    GamepadMapping.__init__(self, 3, target)
        

class GamepadDebugMapping(VehicleMapping):
  def __init__(self,target):
    VehicleMapping.__init__(self,target)

  def GamepadStick2AbsoluteEvent(self, gamepadId, relX, relY):
    #needs calibration
    return E.CameraLookAroundEvent(relX/300.,relY/300.)

  def GamepadButtonPressedEvent(self, gamepadId, button):
    if button == cpp.BUTTON_X: 
      if CONSTS.CAR_DEBUG: 
        print "DebugMode is Off"
        CONSTS.CAR_DEBUG = not CONSTS.CAR_DEBUG
      else: 
        print "DebugMode is On"
        return E.ReloadConstsEvent()  


class GameMapping(Mapping):
  def __init__(self, mappings):
    self.mappings = mappings
  
  def __getattribute__(self, attr):
    if not attr.endswith('Event'): return object.__getattribute__(self, attr)
    def f(*args):
      for mapping in self.mappings:
        m = getattr(mapping, attr, None)
        if m: m(*args)
    return f

