from Mapping import event

MAP = {
  'KeyPressedEvent': {
    (KEY.W,): [event.PlayerAccelerateEvent( 1.0)],
    (KEY.S,): [event.PlayerAccelerateEvent(-1.0)],
    (KEY.A,): [event.PlayerTurnEvent(-1.0)],
    (KEY.D,): [event.PlayerTurnEvent(+1.0)],
  },
  'GamepadStick2RelativeEvent':
    lambda x,y,z: [event.AccelerateEvent(y), event.TurnEvent(x)],
        
}
