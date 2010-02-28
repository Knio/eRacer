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
    self.curState = AIState.DRIVE
  
  def Tick(self,time):
    pos = self.parent.physics.GetPosition()
    self.line.UpdateWaypoint(self.parent.physics.GetPosition())
    cur = self.line.Peek(0)
    game().graphics.graphics.WriteString(
      "cur waypoint: " + str(cur),
      "Verdana", 12, Point3(0,0,0)
    )
    game().graphics.graphics.WriteString(
      "ai speed: " + str(self.parent.physics.GetSpeed()),
      "Verdana", 12, Point3(0,10,0)
    )
    game().graphics.graphics.WriteString(
      "state: " + str(self.curState),
      "Verdana", 12, Point3(0,20,0)
    )
    tx    = self.parent.physics.GetTransform()
    bodyForward = mul0(tx, Z) # forward direction of body
    bodyRight = mul0(tx, X) #vector pointing right of body
    toWaypoint = normalized(cur-pos) # vector towards waypoint
    if self.curState == AIState.DRIVE:
      #print toWaypoint.x, toWaypoint.y, toWaypoint.z
      turnProj = project(toWaypoint, bodyRight)
      
      if length(turnProj) < 0.001:
        self.parent.Turn(0)
      else:
        turnScale = 1.0
        turnSize = length(turnProj) * turnScale # probably use a damping factor
        costheta = dot(turnProj, bodyRight) / length(turnProj)
   
        if 0.999 < costheta < 1.001:#right turn
          self.parent.Turn(turnSize)
        else:
          self.parent.Turn(-turnSize)
      self.parent.Accelerate(0.3)
      #now change state if needed
      if self.parent.physics.GetSpeed() < 10.0 and self.objectInFront(1.0, tx):
        #object close in front has almost stopped us
        self.curState = AIState.STUCK
    
    if self.curState == AIState.STUCK:
      self.parent.Accelerate(-0.5)
      self.parent.Turn(0)
      
      if not self.objectInFront(3.0, tx):
        #nothing in front of us, continue driving normally
        self.curState = AIState.DRIVE
  
#checks whether there is an object in front of the car, within specified distance
#does not check if this is a tiny thing or something that is preventing
#the car from moving 
#takes the distance we should check for, and the transform
#returns true or false
  def objectInFront(self, dist, tx):
    #these positions are just outside the physics box of the car
    bodyForward = mul0(tx, Z)
    frontTopRight = Point3(self.parent.SIZE.x + 0.01, self.parent.SIZE.y + 0.01, self.parent.SIZE.z + 0.01)
    frontTopRight = mul1(tx, frontTopRight)
    dist1 = game().physics.physics.Raycast(frontTopRight, bodyForward)
        
    frontBotRight = Point3(self.parent.SIZE.x + 0.01, -self.parent.SIZE.y - 0.01, self.parent.SIZE.z + 0.01)
    frontBotRight = mul1(tx, frontBotRight)
    dist2 = game().physics.physics.Raycast(frontBotRight, bodyForward)
    
    frontTopLeft = Point3(-self.parent.SIZE.x - 0.01, self.parent.SIZE.y + 0.01, self.parent.SIZE.z + 0.01)
    frontTopLeft = mul1(tx, frontTopLeft)
    dist3 = game().physics.physics.Raycast(frontTopLeft, bodyForward)
    
    frontBotLeft = Point3(-self.parent.SIZE.x - 0.01, -self.parent.SIZE.y - 0.01, self.parent.SIZE.z + 0.01)
    frontBotLeft = mul1(tx, frontBotLeft)
    dist4 = game().physics.physics.Raycast(frontBotLeft, bodyForward)
    
    if dist1 < dist or dist2 < dist or dist3 < dist or dist4 < dist:
      return True
    else:
      return False
    
class AIState:
  DRIVE = 0
  STUCK = 1