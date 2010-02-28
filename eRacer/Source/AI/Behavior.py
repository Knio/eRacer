from Core.Globals import *
from AI.Raceline import Raceline

class Behavior(object):
  def __init__(self, parent):
    self.parent = parent
      
  def Tick(self,time):
    pass
        
        
class PlayerBehavior(Behavior):
  def __init__(self,parent):
    Behavior.__init__(self,parent)
    game().event.Register(self.PlayerAccelerateEvent)
    game().event.Register(self.PlayerTurnEvent)
    game().event.Register(self.PlayerBoostEvent)
    game().event.Register(self.PlayerBrakeEvent)
    
  # control events
  def PlayerBrakeEvent(self, brake):
    self.parent.Brake(brake)

  def PlayerAccelerateEvent(self, changeThrottle):
    self.parent.Accelerate(changeThrottle)

  def PlayerTurnEvent(self, turn):
    self.parent.Turn(turn)
  
  def PlayerBoostEvent(self):
    self.parent.Boost()


class AIBehavior(Behavior):
  def __init__(self, parent, raceline):
    Behavior.__init__(self,parent)
    self.line = raceline
  
  def Tick(self,time):
    pos = self.parent.physics.GetPosition()
    self.line.UpdateWaypoint(self.parent.physics.GetPosition())
    cur = self.line.Peek(0)
    game().graphics.graphics.WriteString(
      "cur waypoint: " + str(cur),
      "Verdana", 12, Point3(0,0,0)
    )