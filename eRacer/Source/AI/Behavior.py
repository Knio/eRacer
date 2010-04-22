from Core.Globals import *


class Behavior(object):
  def __init__(self, parent):
    self.parent = parent
    self.parent.behavior = self
      
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
  
  def PlayerBoostEvent(self, boostState):
    self.parent.Boost(boostState)

  def RespawnCarEvent(self):
    self.parent.resetCar()

class AIBehavior(Behavior):
  def __init__(self, parent, track, gameState, arrow=None):
    Behavior.__init__(self,parent)
    self.gameState = gameState
    self.line = track
    self.arrow = arrow
    self.startDist = 0.0 # the distance around the track at the last check
    self.resetDist = 50.0 # if travelled less than this in interval, reset
    self.resetCheckTime = 5.0 # how often to check if we should reset
    self.resetCounter = -5.0 # don't check for the first 5 seconds
    
    self.curState = AIState.DRIVE
  
  def Tick(self,time):    
    delta = float(time.game_delta) / time.RESOLUTION
    pos = self.parent.physics.GetPosition()
    nowFrame  = self.line.GetFrame(self.parent.trackpos)
    frame1 = self.line.GetFrame(self.parent.trackpos + 20.0)
    frame2 = self.line.GetFrame(self.parent.trackpos + 50.0)
    frame3 = self.line.GetFrame(self.parent.trackpos + 100.0)
    tx    = self.parent.physics.GetTransform()
    bodyForward = mul0(tx, Z) # forward direction of body
    bodyRight = mul0(tx, X) #vector pointing right of body
    bodyUp = mul0(tx, Y)
    
    cur = nowFrame.position
    wantedVec = normalized((frame1.position-pos) * 1.0 +  (frame2.position-pos) * 1.0 + (frame3.position-pos) * 1.0)
    #this is the way we want to go if there are no obstacles in our way and if we are not too close to the wall
    distFromCentre = self.line.GetOffsetFromCentre(pos)
    if self.parent.isShutoff:
      self.curState = AIState.FINISHED
    if self.arrow: 
      self.arrow.transform = Matrix(frame2.position)
      
    if self.curState == AIState.DRIVE:
      turnSize = 0.0
      #first check for obstacles and see if we must avoid them.
      dodgeMode = False
      closestDist = 99999
      closestObs = 0
      frontPos = pos + bodyForward * 5.0 #look from the front of the car
      for obs in self.parent.obstacles:
        toObs = mul1(obs.transform, ORIGIN) - frontPos
        obsProj = projectOnto(toObs, bodyUp)
        costheta = dot(obsProj, bodyForward) / length(obsProj)
        if length(toObs) < closestDist:
          closestDist = length(toObs)
          closestObs = obs
        #obstacle in cone in front of car
        if length(toObs) < 20.0 and costheta > math.sqrt(2)/2 :
          dodgeMode = True
          #print "must dodge"
     # print "closest", closestDist
      if dodgeMode:
        #print "must dodge"
        if distFromCentre > 0: 
        #we are on the right side, so it would be better to go left ot the centre
          self.parent.Turn(-1)
        else:
          self.parent.Turn(1)
      else:
        #print "nothing to dodge"
        #if here, we can drive normally trying to follow the track
        turnProj = project(wantedVec, bodyRight)
     
        turnScale = 5.0
        turnSize = min(1.0, length(turnProj) * turnScale)
        if turnSize < 0.05:
          turnSize = 0
        costheta = dot(turnProj, bodyRight) / length(turnProj)
        fwProj = projectOnto(nowFrame.fw, bodyUp)#check if we are driving into the walls
        fwCosth = dot(fwProj, bodyRight) / length(fwProj)
        #print distFromCentre
       # print "fwcost", fwCosth
        if 0.999 < costheta < 1.001:#right turn
          #if close to right wall and we're going to hit the wall, adjust to centre
          if fwCosth > 0.1 and distFromCentre > self.line.maxX - 15.0:
           # print "too close to right wall"
           # print distFromCentre
            turnSize = turnSize - fwCosth*0.3
        else:
          turnSize = -turnSize
          #if close to left wall and we're going to hit the wall, adjust to centre
          if fwCosth < -0.1 and distFromCentre < self.line.minX + 15.0:
           # print "too close to left wall"
            #print distFromCentre
            #note that here fwCosth is negative, so make it positive in order to turn right
            turnSize = turnSize + fwCosth*-0.3
        cappedTurn = min(max(turnSize, -1.0), 1.0)
        #print cappedTurn
        self.parent.Turn(cappedTurn)

      worstPlayerPos = self.parent.frame.dist
     # print "my pos:", self.parent.frame.dist
      for vehicle in self.gameState.playerVehicles:
        #print "players dist:", vehicle.frame.dist
        if worstPlayerPos > vehicle.frame.dist:
          worstPlayerPos = vehicle.frame.dist
      distAhead = self.parent.frame.dist - worstPlayerPos
      #print "dist ahead last player:", distAhead
      if(distAhead > 1000):
        dodgeMode = True
        #print "way ahead, slow to 0.2"
        self.parent.Accelerate(0.6)
        self.parent.Boost(False)
      elif(distAhead > 500):
        dodgeMode = True
        #print "pretty far ahead, slow to 0.5"
        self.parent.Accelerate(0.75)
        self.parent.Boost(False)
      elif(distAhead > 200):
        dodgeMode = True
        #print "a little ahead, slow down to 0.8"
        self.parent.Accelerate(0.9)
        self.parent.Boost(False)
      else:
        self.parent.Accelerate(1.0)
      #basic boost code: we don't need to turn off boost until the turn becomes large
      #print turnSize
      distAhead = self.line.GetOffsetFromCentre(pos + bodyForward * 50.0)
      if turnSize < 0.1 and self.parent.boostFuel > 2 and not dodgeMode:
        if distAhead < self.line.maxX and distAhead > self.line.minX:
        #make sure we won't jump off the edge
          #print "boost"
          self.parent.Boost(True)
      if turnSize > 0.5:
        #print "boost off"
        self.parent.Boost(False)
        
      self.ResetChecker(delta, nowFrame)

      #now change state if needed
      if self.parent.physics.GetSpeed() < 1.0 and self.objectInFront(1.0, tx):
        #object close in front has almost stopped us
        self.curState = AIState.STUCK
       
    if self.curState == AIState.STUCK:
      self.parent.Accelerate(-1.0)
      self.parent.Turn(0)
      self.ResetChecker(delta, nowFrame)
      if not self.objectInFront(8.0, tx):
        #nothing in front of us, continue driving normally
        self.curState = AIState.DRIVE
        
    if self.curState == AIState.FINISHED:
      self.parent.Turn(0)
      self.parent.Accelerate(0)
      self.parent.Boost(False)
#checks whether there is an object in front of the car, within specified distance
#does not check if this is a tiny thing or something that is preventing
#the car from moving 
#takes the distance we should check for, and the transform
#returns true or false
  def objectInFront(self, dist, tx):
    #these positions are just outside the physics box of the car
    #should match with the values in the BodyObject
    bodyForward = mul0(tx, Z)
    eps = -0.001
   # frontTopRight = Point3(self.parent.SIZE.x + eps, self.parent.SIZE.y + eps, self.parent.SIZE.z + eps)
    frontTopRight = Point3(2.2, 1.11, 6.1)
    frontTopRight = mul1(tx, frontTopRight)
    dist1 = game().physics.physics.Raycast(frontTopRight, bodyForward)
    #print "d1", dist1
        
    #frontBotRight = Point3(self.parent.SIZE.x + eps, -self.parent.SIZE.y - eps, self.parent.SIZE.z + eps)
    frontBotRight = Point3(2.2, -1.11, 6.1)
    frontBotRight = mul1(tx, frontBotRight)
    dist2 = game().physics.physics.Raycast(frontBotRight, bodyForward)
    #print "d2", dist2
    
   # frontTopLeft = Point3(-self.parent.SIZE.x - eps, self.parent.SIZE.y + eps, self.parent.SIZE.z + eps)
    frontTopLeft = Point3(-2.2, 1.11, 6.1)
    frontTopLeft = mul1(tx, frontTopLeft)
    dist3 = game().physics.physics.Raycast(frontTopLeft, bodyForward)
    #print "d3", dist3
    
   # frontBotLeft = Point3(-self.parent.SIZE.x - eps, -self.parent.SIZE.y - eps, self.parent.SIZE.z + eps)
    frontBotLeft = Point3(-2.2, -1.11, 6.1)
    frontBotLeft = mul1(tx, frontBotLeft)
    dist4 = game().physics.physics.Raycast(frontBotLeft, bodyForward)
    #print "d4", dist4
    
    if dist1 < dist or dist2 < dist or dist3 < dist or dist4 < dist:
      return True
    else:
      return False
    
    
  def ResetChecker(self, delta, nowFrame):
    self.resetCounter = self.resetCounter + delta
    if(self.resetCounter > self.resetCheckTime):
      distTravelled = nowFrame.dist - self.startDist
      #print "travelled" , distTravelled
      
      if(distTravelled < self.resetDist):
        #print "must reset, we didn't travel far"
        self.parent.resetCar()
      
      self.startDist = nowFrame.dist
      self.resetCounter = 0.0
    
  
class AIState:
  DRIVE = "DRIVE"
  STUCK = "STUCK"
  FINISHED = "FINISHED"
