from Core.Globals import *

class Vehicle(Prop):
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
    
  def __init__(self, name, track, tx, modelnum=1):
    self.ReloadedConstsEvent()

    Prop.__init__(
      self,
      MeshNode(name),
      'Racer1.x',
      cpp.CarBody(True, self.MASS),
      tx,
      modelnum,
    )
    self.behavior = None
    self.trackpos = -1.0
    self.track    = track
    self.name     = name
    self.frame = cpp.Frame(mul1(tx, ORIGIN), mul0(tx, Y), mul0(tx, Z), 0.0)
    self.resetFrame = self.frame
    self.velocity = ORIGIN
    
    self.physics.SetCentreOfMass(self.MASS_CENTRE)
    self.physics.SetGroup(cpp.CAR)
  
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
    
    self.sound = cpp.SoundFx();
    self.sound.looping  = True
    self.sound.is3D     = True
    self.sound.isPaused = True
    self.sound.volume   = 250
    self.sound.minDist  = 50

    game().sound.sound.LoadSoundFx("Resources/Sounds/motorsound2.wav", self.sound)

    game().event.Register(self.ReloadedConstsEvent)

    
  # control events
  def Brake(self, brake):
    self.brake = brake

  def Accelerate(self, changeThrottle):
    self.throttle = changeThrottle

  def Turn(self, turn):
    self.steerPos = turn
  
  def Boost(self):
    self.boosting = 2.0
  
  def Tick(self, time):
    
    # if not time.game_delta:
    #   return
    
    phys  = self.physics
    tx    = phys.GetTransform()
    delta = float(time.game_delta) / time.RESOLUTION
    worldpos   = mul1(tx, ORIGIN)
    self.velocity = phys.GetVelocity()
    
    
    frame = self.track.GetFrame(worldpos, self.trackpos)
    self.frame = frame
    self.trackpos = frame.dist
    
    up    = frame.up
    fw    = frame.fw
  
    lapcount = int(self.trackpos / self.track.dist)
    if (lapcount > self.lapcount):
      game().event.LapEvent(self, lapcount)
    self.lapcount = lapcount
  
    if self.behavior: 
      self.behavior.Tick(time)
    

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
    if 0 < tempD < 20:
      worldroadnormal = up      
      gravity = worldroadnormal * (-CONSTS.CAR_GRAVITY * self.MASS)
      if delta: phys.AddWorldForceAtLocalPos(gravity, self.MASS_CENTRE)
    
    vel = phys.GetLocalPointWorldVelocity(ORIGIN)
    
    self.sound.isPaused = delta==0
    
    self.sound.position = mul1(tx, ORIGIN)
    self.sound.velocity = ORIGIN #vel
    

    
    self.sound.pitch = max(50000, int(50000 * length(vel) / 60.0))
    if self.crashtime > 0:
      self.sound.pitch = int(54100 * self.throttle + self.sound.pitch)
    game().sound.sound.UpdateSoundFx(self.sound)
    
    crashed = True
    for i,localpos in enumerate(self.WHEELS):
      localapplypoint = Point3(localpos.x, -1, localpos.z)
      # wheel vectors in world space
      worldpos   = mul1(tx, localpos)
      worldvel   = phys.GetLocalPointWorldVelocity(localpos)
      
      # smooth out the velocity
      self.wheelvel[i] = self.wheelvel[i] * (1-CONSTS.WHEELVEL_ALPHA) + worldvel * CONSTS.WHEELVEL_ALPHA
      avgworldvel = self.wheelvel[i]
      
      
      #raycast down from suspension point
      upamount = 2.0
      worldroadnormal = Vector3()
      localsuspoint   = Point3(localpos.x, localpos.y + upamount, localpos.z)
      worldsuspoint   = mul1(tx, localsuspoint)
      # dist = dist = game().physics.physics.Raycast(worldsuspoint, dir, worldroadnormal) - upamount
      dist = dot(up, (worldsuspoint - frame.position)) - upamount
      if length(worldsuspoint - frame.position) > 26:
        dist = 1e99
      
      disp = (self.DISPLACEMENT - dist)

      # use track normal and not physx
      worldroadnormal = up     
      
      D = CONSTS.CAR_DEBUG == self.id
      _debug = [self.DEBUG[i]]
      def debug(s):
        game().graphics.graphics.WriteString(s, "Verdana", 12, _debug[0])
        _debug[0] = _debug[0] + Point3(0, 15, 0)
      
      if D: debug("W-R: %6.3f" % dist)
      if D: debug("Disp: %6.3f" % disp)
      
      # check for invalid distances
      if dist < localpos.y:
        if D: debug("BAD")
        continue
      
      if dist < localpos.y+1.0:
        if D: debug("ROAD")
        # do we want to still do forces?
      
      if disp > 0:
        if D: debug("GOOD!")
      
      else:
        if dist < 20:
          if D: debug("JUMP")
          crashed = False
        else:
          if D: debug("AIR")
        continue
      
      crashed = False
      
      # spring force
      downforce = worldroadnormal * disp * self.SPRING_K * self.SPRING_MAGIC
      #print delta, disp, self.SPRING_K, self.SPRING_MAGIC
      #print force.x, force.y, force.z
      if D: debug("Spring: %2e" % dot(downforce, worldroadnormal))
      if delta: phys.AddWorldForceAtLocalPos(downforce, localapplypoint)
      
      # shock absorber forces
      linearvel = dot(worldvel, worldroadnormal)
      linearvel = linearvel > 0 and -(linearvel**0.95) or (-linearvel)**0.95
      # linearvel = max(linearvel, 0.0)
      slowforce = worldroadnormal * linearvel * self.DAMPING * self.DAMPING_MAGIC
      if D: debug("Shock: %6g %s" % (dot(slowforce, worldroadnormal), repr(slowforce)))
      if delta: phys.AddWorldForceAtLocalPos(slowforce, localapplypoint)
      
      weight = length(downforce+slowforce)
      if D: debug("Weight: %6.2g" % weight)
      # do accelleration
      
      forwardSpeed = self.GetWheelSpeed(delta, weight)
      #forwardSpeed = self.GetWheelSpeed(delta)
      if D: debug("FW: %6.2f" % forwardSpeed)
      
      # TODO modify Z for steering
      # direction of the wheel on the surface of the road
      # front wheel turns
      
      angle = self.turning * min(1.,(60. / max(1.,length(avgworldvel))**1.4))
      if D: debug("angle: %6.2f" % angle)
      if i < 2: turning = Matrix(ORIGIN, angle, Y)
      else:     turning = Matrix()
      
      # +Z is forward in the local space
      # wheel rolling direction
      worldrollingdir = mul0(tx, mul0(turning, Z))
      # motion along the wheel's rolling direction
      worldrollingvel = project(avgworldvel, worldrollingdir)
      worldrollingvelroad = projectOnto(worldrollingvel, worldroadnormal)
      
      # motion the wheel WANTS to be going
      worldforwardvel = mul0(tx, mul0(turning, Z * forwardSpeed))
      worldforwardvelroad = projectOnto(worldforwardvel, worldroadnormal)
      
      
      # wheel's current velocity projected on the surface of the road
      worldvelroad = projectOnto(avgworldvel,  worldroadnormal)
      # assert(abs(dot(worldvelroad, worldroadnormal)) < 1e-3), dot(worldvelroad, worldroadnormal)
      
      # difference of where the wheel wants to go, and where it is really going.
      # this is where I start making shit up
      
      # lateral force by friction
      frictionforce = (worldrollingvelroad - worldvelroad)
      if D: debug('Fr: '+repr(frictionforce))
      # forward force by engine
      powerforce    = worldforwardvelroad
      if self.brake:  powerforce    = -worldrollingvelroad
      totalforce    = frictionforce + powerforce
      # assert(abs(dot(totalforce, worldroadnormal)) < 1e-3), dot(totalforce, worldroadnormal)
      
      
      # staticfrictionmax = self.FRICTION_SLIDING * weight # weight on wheel
      
      # sliding
      if self.sliding[i]:
        # never used so far
        if D: debug("SLIDING")

        if length(totalforce) < 1.0:
          # back to static
          if D: debug("STATIC2")
          self.sliding[i] = False
        
        totalforce = normalized(totalforce) * CONSTS.FRICTION_SLIDING * weight
        ##############
        
      # static
      else:
        if length(totalforce) > 1.0:
          # we're sliding!
          totalforce = normalized(totalforce)
          if D: debug("SLIDING2")
          self.sliding[i] = True
        else:
          if D: debug("STATIC")
        
        totalforce = totalforce * CONSTS.FRICTION_STATIC  * weight
              
      if D: debug('To: '+repr(totalforce))
      if D: debug("Power:  %6.2f" % length(totalforce))
      # if D: debug("Static: %6.2f" % staticfrictionmax)
      # debug("P.N: %6.2f" % dot(totalforce, worldroadnormal))
      
      # continue
      if delta: 
        reverseVel = normalized(bodyVel) * -1.0
        rollFrict = reverseVel * (weight*CONSTS.FRICTION_ROLL) # slow us down a little
        totalforce = totalforce + rollFrict
        phys.AddWorldForceAtLocalPos(totalforce, localapplypoint)
      
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


    self.boosting = max(0, self.boosting - delta)

    if self.boosting > 0:
      game().graphics.graphics.WriteString(
      "BOOST %2.2f" % (self.boosting),
      "Verdana", 50, Point3(255,500,0)
      )


    
    Prop.Tick(self, time)
    

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
    if self.boosting:
      forceMag = self.MAX_ENG_FORCE * CONSTS.BOOST_MULT
    
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
    
