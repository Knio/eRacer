from Core.Globals import *
from Mapping      import E


MAP = {
  'KeyPressedEvent': {
    (KEY.W,): [E.PlayerAccelerateEvent(+1.0)],
    (KEY.S,): [E.PlayerAccelerateEvent(-1.0)],
    (KEY.A,): [E.PlayerTurnEvent(-1.0)],
    (KEY.D,): [E.PlayerTurnEvent(+1.0)],
  },
  'KeyReleasedEvent': {
    (KEY.W,): [E.PlayerAccelerateEvent(-1.0)],
    (KEY.S,): [E.PlayerAccelerateEvent(+1.0)],
    (KEY.A,): [E.PlayerTurnEvent(+1.0)],
    (KEY.D,): [E.PlayerTurnEvent(-1.0)],
  },
  'GamepadStick2RelativeEvent':
    lambda x,y,z: [E.AccelerateEvent(y/1000.), E.TurnEvent(x/1000.)],
        
}
