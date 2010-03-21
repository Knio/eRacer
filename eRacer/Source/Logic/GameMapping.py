from Core.Globals import *
from Mapping      import Mapping, E

class VehicleMapping(Mapping):
  def __init__(self,target):
    self.target = target

  def KeyPressedEvent(self, key):
    pass
  
  def KeyReleasedEvent(self, key):
    pass
  
  def MouseMovedEvent(self, relX, relY):
    pass
  
  def GamepadStick1AbsoluteEvent(self, x, y):
    pass

  def GamepadStick2AbsoluteEvent(self, relX, relY):
    pass
  
  def GamepadTriggerAbsoluteEvent(self, z):
    pass
  
  def GamepadButtonPressedEvent(self, button):
    pass
  
  def GamepadButtonReleasedEvent(self, button):
    pass            
        
        
class Keyboard1Mapping(VehicleMapping):
  def __init__(self,target):
        VehicleMapping.__init__(self,target)
  
  def KeyPressedEvent(self, key):
    if   key == KEY.W:      return self.target.PlayerAccelerateEvent  (+1.0)
    elif key == KEY.S:      return self.target.PlayerAccelerateEvent  (-1.0)
    elif key == KEY.LSHIFT: return self.target.PlayerBrakeEvent       (True)
    elif key == KEY.A:      return self.target.PlayerTurnEvent        (-1.0)
    elif key == KEY.D:      return self.target.PlayerTurnEvent        (+1.0)
    elif key == KEY.SPACE:  return self.target.PlayerBoostEvent       (True)
    elif key == KEY.C:      return E.CameraChangedEvent     (    )
    elif key == KEY.ESCAPE: return E.PauseEvent             (    )
    elif key == KEY.T:      return E.RespawnCarEvent        (    )
    
  def KeyReleasedEvent(self, key):
    if   key == KEY.W:     return self.target.PlayerAccelerateEvent ( 0)
    elif key == KEY.S:     return self.target.PlayerAccelerateEvent ( 0)
    elif key == KEY.LSHIFT:return self.target.PlayerBrakeEvent      (False)
    elif key == KEY.A:     return self.target.PlayerTurnEvent       ( 0)
    elif key == KEY.D:     return self.target.PlayerTurnEvent       ( 0)
    elif key == KEY.SPACE:  return self.target.PlayerBoostEvent     (False)
        
class Keyboard2Mapping(VehicleMapping)     :   
  def __init__(self,target):
    VehicleMapping.__init__(self,target)
        
  def KeyPressedEvent(self, key):
    if   key == KEY.UP:         return self.target.PlayerAccelerateEvent  (+1.0)
    elif key == KEY.DOWN:       return self.target.PlayerAccelerateEvent  (-1.0)
    elif key == KEY.RCONTROL:   return self.target.PlayerBrakeEvent       (True)
    elif key == KEY.LEFT:       return self.target.PlayerTurnEvent        (-1.0)
    elif key == KEY.RIGHT:      return self.target.PlayerTurnEvent        (+1.0)
    elif key == KEY.NUMPAD0:    return self.target.PlayerBoostEvent       (True)
    elif key == KEY.DECIMAL:    return E.CameraChangedEvent     (    )
    elif key == KEY.PAUSE:      return E.PauseEvent             (    )
    elif key == KEY.RSHIFT:     return E.RespawnCarEvent        (    )
    
    
  def KeyReleasedEvent(self, key):
    if   key == KEY.UP:         return self.target.PlayerAccelerateEvent ( 0)
    elif key == KEY.DOWN:       return self.target.PlayerAccelerateEvent ( 0)
    elif key == KEY.RCONTROL:   return self.target.PlayerBrakeEvent      (False)
    elif key == KEY.LEFT:       return self.target.PlayerTurnEvent       ( 0)
    elif key == KEY.RIGHT:      return self.target.PlayerTurnEvent       ( 0)
    elif key == KEY.NUMPAD0:    return self.target.PlayerBoostEvent     (False)

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
      

class Gamepad1Mapping(VehicleMapping):
  def __init__(self,target):
    VehicleMapping.__init__(self,target)

  def GamepadStick1AbsoluteEvent(self, x, y):
    return self.target.PlayerTurnEvent(x/1000.0)  

  def GamepadTriggerAbsoluteEvent(self, z):
    return self.target.PlayerAccelerateEvent(z/-1000.0)
    
  def GamepadButtonPressedEvent(self, button):
    if   button == cpp.BUTTON_START:   return E.PauseEvent()
    elif button == cpp.BUTTON_A:       return self.target.PlayerBoostEvent(True)
    elif button == cpp.BUTTON_B:       return self.target.PlayerBrakeEvent(True)
    elif button == cpp.BUTTON_Y:       return E.CameraChangedEvent()

  def GamepadButtonReleasedEvent(self, button):
    if button == cpp.BUTTON_B:       return self.target.PlayerBrakeEvent(False)
    elif button == cpp.BUTTON_A:       return self.target.PlayerBoostEvent(False)

class GamepadDebugMapping(VehicleMapping):
  def __init__(self,target):
    VehicleMapping.__init__(self,target)

  def GamepadStick2AbsoluteEvent(self, relX, relY):
    #needs calibration
    return E.CameraLookAroundEvent(relX/300.,relY/300.)

  def GamepadButtonPressedEvent(self, button):
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
  
  def KeyPressedEvent(self, key):
    for mapping in self.mappings:
      mapping.KeyPressedEvent(key)
    
  def KeyReleasedEvent(self, key):
    for mapping in self.mappings:
      mapping.KeyReleasedEvent(key)
    
  def MouseMovedEvent(self, relX, relY):
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
    
  def GamepadStick1AbsoluteEvent(self, x, y):
    for mapping in self.mappings:
      mapping.GamepadStick1AbsoluteEvent(key)

  def GamepadStick2AbsoluteEvent(self, relX, relY):
    for mapping in self.mappings:
      mapping.GamepadStick2AbsoluteEvent(key)
        
  def GamepadTriggerAbsoluteEvent(self, z):
    for mapping in self.mappings:
      mapping.GamepadTriggerAbsoluteEvent(key)
    
  def GamepadButtonPressedEvent(self, button):
    for mapping in self.mappings:
      mapping.GamepadButtonPressedEvent(key)
    
  def GamepadButtonReleasedEvent(self, button):
    for mapping in self.mappings:
      mapping.GamepadButtonReleasedEvent(key)
