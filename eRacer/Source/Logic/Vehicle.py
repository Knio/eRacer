from Core.Globals import *

class Vehicle(Model):
  SIZE    = Vector3(2.5, 1, 4.5) # "radius" (double for length)
  WHEELS  = [ # location of wheels in object space
    Point3(-2.5, -2.0,  4.5), # front left
    Point3( 2.5, -2.0,  4.5), # front right
    Point3(-2.5, -2.0, -4.5), # back left
    Point3( 2.5, -2.0, -4.5), # back right
  ]
  DEBUG   = [ # location of debug strings in screen space
    Point3(240,  80, 0),
    Point3(440,  80, 0),
    Point3(240, 340, 0),
    Point3(440, 340, 0),
  ]
  DISPLACEMENT = 0.40  # from wheel rest position
  
  def ReloadedConstsEvent(self):
    self.MASS = CONSTS.CAR_MASS
    # can't set mass or centre of mass on the fly due to Physx
    # self.physics.SetMass(self.MASS) #setting the mass breaks the suspension for some reason
    # maybe physx is still running
    self.DISPLACEMENT     = CONSTS.CAR_DISPLACEMENT
    self.SPRING_MAGIC     = CONSTS.SPRING_MAGIC
    self.DAMPING_MAGIC    = CONSTS.DAMPING_MAGIC
    self.MASS_CENTRE      = Point3(CONSTS.MASS_CENTRE_X, CONSTS.MASS_CENTRE_Y, CONSTS.MASS_CENTRE_Z)
    self.REV_ALPHA        = CONSTS.REV_ALPHA
    self.TURN_ALPHA       = CONSTS.TURN_ALPHA
    self.DRAG_COEFF       = CONSTS.DRAG_COEFF
    self.MAX_ENG_FORCE    = CONSTS.MAX_ENG_FORCE       
    self.MAX_BRAKE_FORCE  = CONSTS.MAX_BRAKE_FORCE  
    self.SPRING_K         = (CONSTS.CAR_MASS * CONSTS.CAR_GRAVITY) / (len(self.WHEELS) * self.DISPLACEMENT)
    self.DAMPING          = 2.0 * math.sqrt(self.SPRING_K * self.MASS)
    print 'SPRING_K', self.SPRING_K
    print 'DAMPING',  self.DAMPING
    
  def __init__(self, name, track, tx, modelnum=1):
    self.ReloadedConstsEvent()

    Model.__init__(
      self,
      name,
      'Racer.x',
      cpp.CarBody(self.MASS),
      tx,
      modelnum,
    )
    self.behavior = None
    self.maxtrackpos = -1.0
    self.trackpos = -1.0
    self.lasttrackpos = -1.0
    self.track    = track
    self.name     = name
    self.frame = cpp.Frame(mul1(tx, ORIGIN), mul0(tx, Y), mul0(tx, Z), 0.0)
    self.resetFrame = self.frame
    self.velocity = ORIGIN
    self.isShutoff = False
    
    self.physics.SetCentreOfMass(self.MASS_CENTRE)
    self.physics.SetId(self.id)
    
    # tip of the car - should not be hardcoded ideally
    self.tip = Point3(0, 0, 1.11+9.03/2+1.2)

  
    self.throttle = 0.      # position of the throttle on game controller from 0 to 1
    self.brake    = False   # brake button
    self.steerPos = 0.      # position of the control stick from left to right ranges from -1 to +1

    self.acceleration = 0.  # physics engine setting.   from 0 to 1
    self.turning      = 0.  # physics steering setting. from 0 to 1
    
    self.sliding = [False] * len(self.WHEELS) # static vs sliding state of each wheel
    self.wheelvel = [ORIGIN, ORIGIN, ORIGIN, ORIGIN]
    self.crashtime = 0      # time since wheels were last in contact with the ground
    
    self.boosting = 0.
    self.lapcount = 0

    self.boostFuel = 5.
    
    self.sound = cpp.SoundFx();
    self.sound.isLooping  = True
    self.sound.is3D     = True
    self.sound.isPaused = True
    self.sound.volume   = 128
    self.sound.minDist  = 50
    
    self.obstacles = []
    self.stealingBeams = {}

    game().sound.sound.LoadSoundFx("EngineSound.wav", self.sound)

    game().event.Register(self.ReloadedConstsEvent)    

  # control events
  def Brake(self, brake):
    self.brake = brake

  def Accelerate(self, changeThrottle):
    self.throttle = changeThrottle

  def Turn(self, turn):
    self.steerPos = turn
  
  def Boost(self, boostState):
    tx = self.physics.GetTransform()
    if boostState == True and self.boostFuel > 0.5:
      normal = Vector3(0, 0, 0)
      dist = game().physics.physics.Raycast(mul1(tx, ORIGIN), mul0(tx, -Y), normal)
      if dist < 3.0 and self.boosting == 0:
        self.boostFuel = self.boostFuel - 0.5
        pushForce = normalized(Vector3(0,1,1)) * 280000 
        self.physics.AddLocalImpulseAtLocalPos(pushForce, self.MASS_CENTRE)
      self.boosting = 1
    else:
      self.boosting = 0
  
  def Tick(self, time):
    
    if not time.game_delta:
      self.sound.isPaused = True
      game().sound.sound.UpdateSoundFx(self.sound)
      return
    
    delta = float(time.game_delta) / time.RESOLUTION
    
    phys  = self.physics
    tx    = phys.GetTransform()
    worldpos   = mul1(tx, ORIGIN)
    self.velocity = phys.GetVelocity()
    
    frame = self.track.GetFrame(worldpos, self.trackpos)
    self.frame = frame
    self.lasttrackpos = self.trackpos
    self.trackpos = frame.dist
    if (self.trackpos - self.lasttrackpos) >= (self.track.dist-100):
      self.trackpos = self.trackpos - self.track.dist
      frame = self.track.GetFrame(worldpos, self.trackpos)
      self.frame = frame
      self.lasttrackpos = self.trackpos
      self.trackpos = frame.dist
    
    up    = frame.up
    fw    = frame.fw
  
    lapcount = int(self.trackpos / self.track.dist)
    if (lapcount > self.lapcount):
        game().event.LapEvent(self, lapcount)
    self.lapcount = lapcount
  
    if self.behavior: 
      self.behavior.Tick(time)
    
    # remove obstacles - they are not needed any more
    self.obstacles = []
    

    self.PrintDebug()
    # do engine/brake/steering/user input forces    
    
    alphaa = math.pow(self.REV_ALPHA,  delta)
    alphat = math.pow(self.TURN_ALPHA, delta)
    
    self.acceleration = (alphaa)*self.acceleration + (1-alphaa)*self.throttle
    self.turning      = (alphat)*self.turning      + (1-alphat)*self.steerPos

    # suspension axis (pointing down from car)
    axis  = mul0(tx, -Y)
    
    # drag
    bodyVel = phys.GetVelocity();
    dragForceMag = self.DRAG_COEFF * length(bodyVel) ** 2
    dragDir = normalize(bodyVel) * -1.0
    dragForce = dragDir * dragForceMag
    if delta: phys.AddWorldForceAtLocalPos(dragForce, self.MASS_CENTRE)
    #print dragForce.x, dragForce.y, dragForce.z
    
    # gravity
    
    worldroadnormal = Vector3()
    center = Vector3(0, -1.1, 0)
    
    dir = mul0(tx, -Y)
    normalize(dir)
    #if 0 < phys.RaycastDown(mul1(tx, center), worldroadnormal) < 20:
    tempD = game().physics.physics.Raycast(mul1(tx, center), dir, worldroadnormal)
    #print tempD
    if 0 < tempD < 40:
      worldroadnormal = up      
      gravity = worldroadnormal * (-CONSTS.CAR_GRAVITY * self.MASS)
      if delta: phys.AddWorldForceAtLocalPos(gravity, self.MASS_CENTRE)
    
    vel = phys.GetLocalPointWorldVelocity(ORIGIN)
    
    self.sound.isPaused = delta==0
    if self.isShutoff == True:
      self.sound.isPaused = True;
    
    self.sound.position = mul1(tx, ORIGIN)
    self.sound.velocity = ORIGIN #vel
    

    
    self.sound.pitch = max(50000, int(50000 * length(vel) / 60.0))
    if self.crashtime > 0:
      self.sound.pitch = int(54100 * self.throttle + self.sound.pitch)
    game().sound.sound.UpdateSoundFx(self.sound)
    
    crashed = True
    self.sumHeight = 0.
    for i,localpos in enumerate(self.WHEELS):
      
      ################## NEW CODE
      # if CONSTS.CAR_CPP:
      dist = phys.SimWheel(
        i, localpos, frame,
        self.turning, self.acceleration, self.brake
      )
      
      if -2 < dist < 20: crashed = False
      
    ##################################################################################
    # no wheels are touching the ground.
    # reset the car
    if not crashed:
      self.crashtime = 0
      self.resetFrame = frame
    else:
      self.crashtime += delta
    
    if self.crashtime > 3: # or car stopped? ## why stopped?
      self.crashtime = 0
      print "Crash! resetting car"
      self.resetCar()

    if self.brake and phys.GetSpeed() < 2:
      pass
      # phys.SetVelocity(ORIGIN)
      # phys.SetAngVelocity(ORIGIN)


    #self.boosting = max(0, self.boosting - delta)
    #if self.boosting > 0:
    #  game().graphics.graphics.WriteString(
    #  "BOOST %2.2f" % (self.boosting),
    #  "Verdana", 50, Point3(255,500,0)
    #  )

    if self.boosting > 0:
      self.boostFuel = max( 0, self.boostFuel - delta )
      if self.boostFuel == 0:
        self.boosting = 0

      tx = self.physics.GetTransform()
      normal = Vector3(0, 0, 0)
      dist = game().physics.physics.Raycast(mul1(tx, ORIGIN), mul0(tx, -Y), normal)
      if dist < 3.0:
        pushForce = normalized(Vector3(0,-1.2,1)) * 6000
        phys.AddLocalImpulseAtLocalPos(pushForce, self.MASS_CENTRE)
    else:    
      self.boostFuel = min( 5, self.boostFuel + delta/3 )
      
    Model.Tick(self, time)
    

  def PrintDebug(self):
    if not CONSTS.CAR_DEBUG == self.id: return
    # print debug info
    phys = self.physics
    vel = phys.GetVelocity()
    game().graphics.graphics.WriteString(
      "Vel: (%6.2f %6.2f %6.2f)" % (vel.x, vel.y, vel.z),
      "Verdana", 12, Point3(380,280,0)
    )

    game().graphics.graphics.WriteString(
      "Speed: %6.2f" % phys.GetSpeed(),
      "Verdana", 12, Point3(380,300,0)
    )
    
    game().graphics.graphics.WriteString(
      "Throttle:  %4.2f (%4.2f)" % (self.throttle, self.acceleration),
      "Verdana", 12, Point3(380,320,0)
    )
    
    game().graphics.graphics.WriteString(
      "Steer:  %4.2f (%4.2f)" % (self.steerPos, self.turning),
      "Verdana", 12, Point3(380,340,0)
    )
    
  
  # why does this depend on the down force?
  
  #get the speed the car wants to add to this wheel in the forward direction
  #this will be due to the braking or acceleration the user wants
  #needs the weight on this tire
  #returns a floating number, if negative, then a braking force was applied
  
  def GetWheelSpeed(self, timeStep, weight):
    gravityMag = 9.81 #should change based on track segment
    forceMag = self.MAX_ENG_FORCE * self.acceleration
    #if self.boosting:
    #  forceMag = self.MAX_ENG_FORCE * CONSTS.BOOST_MULT
    
    if self.brake: return 0.
    
    if self.physics.GetSpeed() < 1:
      brakeMag =0
    else:
      brakeMag = self.MAX_BRAKE_FORCE * self.brake
    
    massOnTire = weight / gravityMag
    
    if forceMag > 0:
      speedDelta = (forceMag-brakeMag) / massOnTire
    else:
      speedDelta = (forceMag+brakeMag) / massOnTire
    return speedDelta
    
        
  def resetCar(self):
    print 'Reset Car'
    phys  = self.physics
    self.resetFrame.position = self.resetFrame.position + self.resetFrame.up * 3.0
    orient = Matrix(self.resetFrame.position, self.resetFrame.up, self.resetFrame.fw)
    phys.SetOrientation(orient)
    phys.SetAngVelocity(ORIGIN)
    phys.SetPosition(self.resetFrame.position)
    phys.SetVelocity(self.resetFrame.fw * 10.0)
    
