from Core.Globals import *
from Mapping      import Mapping, E

class GameMapping(Mapping):
  def KeyPressedEvent(self, key):
    if   key == KEY.W:      return E.PlayerAccelerateEvent  (+1.0)
    elif key == KEY.S:      return E.PlayerAccelerateEvent  (-1.0)
    elif key == KEY.LSHIFT: return E.PlayerBrakeEvent       (True)
    elif key == KEY.A:      return E.PlayerTurnEvent        (-1.0)
    elif key == KEY.D:      return E.PlayerTurnEvent        (+1.0)
    elif key == KEY.CAPITAL:return E.PlayerBoostEvent       (    )
    elif key == KEY.UP:     return E.CameraAccelerateEvent  (+1.0)
    elif key == KEY.DOWN:   return E.CameraAccelerateEvent  (-1.0)
    elif key == KEY.LEFT:   return E.CameraStrafeEvent      (-1.0)
    elif key == KEY.RIGHT:  return E.CameraStrafeEvent      (+1.0)
    elif key == KEY.C:      return E.CameraChangedEvent     (    )
    elif key == KEY.ESCAPE: return E.PauseEvent             (    )
    elif key == KEY.SPACE:  return E.PlayJaguarSoundEvent   (    )

  def KeyReleasedEvent(self, key):
    if   key == KEY.W:     return E.PlayerAccelerateEvent ( 0)
    elif key == KEY.S:     return E.PlayerAccelerateEvent ( 0)
    elif key == KEY.LSHIFT:return E.PlayerBrakeEvent      (False)
    elif key == KEY.A:     return E.PlayerTurnEvent       ( 0)
    elif key == KEY.D:     return E.PlayerTurnEvent       ( 0)
    elif key == KEY.UP:    return E.CameraAccelerateEvent (-1.0)
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
    
  def GamepadStick1AbsoluteEvent(self, x, y):
    return E.PlayerTurnEvent(x/1000.0)

  def GamepadStick2AbsoluteEvent(self, relX, relY):
    #needs calibration
    return E.CameraLookAroundEvent(relX/300.,relY/300.)
        
  def GamepadTriggerAbsoluteEvent(self, z):
    return E.PlayerAccelerateEvent(z/-1000.0)
    
  def GamepadButtonPressedEvent(self, button):
    if   button == eRacer.BUTTON_START:   return E.PauseEvent()
    elif button == eRacer.BUTTON_A:       return E.PlayerBoostEvent()
    elif button == eRacer.BUTTON_B:       return E.PlayerBrakeEvent(True)
    elif button == eRacer.BUTTON_Y:       return E.CameraChangedEvent()
    elif button == eRacer.BUTTON_X: 
		if CONSTS.CAR_DEBUG: 
			print "DebugMode is Off"
			CONSTS.CAR_DEBUG = not CONSTS.CAR_DEBUG
		else: 
			print "DebugMode is On"
			return E.ReloadConstsEvent()  
    
  def GamepadButtonReleasedEvent(self, button):
    if button == eRacer.BUTTON_B:       return E.PlayerBrakeEvent(False)
