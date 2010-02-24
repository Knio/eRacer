from Core.Globals import *
class Vehicle(Entity):
  MODEL   = "Racer.x"
  SIZE    = Vector3(3, 1, 4.5) # "radius" (double for length)
  WHEELS  = [ # location of wheels in object space
    Point3(-2, -1.5,  4), # front left
    Point3( 2, -1.5,  4), # front right
    Point3(-2, -1.5, -4), # back left
    Point3( 2, -1.5, -4), # back right
  ]
  DEBUG   = [ # location of debug strings in screen space
    Point3(280, 180, 0),
    Point3(480, 180, 0),
    Point3(280, 380, 0),
    Point3(480, 380, 0),
  ]
  DISPLACEMENT = 0.30  # from wheel rest position  
  INITIAL_POS = Vector3(80, 2, 0)

  def __init__(self, scene):
    Entity.__init__(self)
    
    self.MASS = CONSTS.CAR_MASS
    self.SPRING_MAGIC = CONSTS.SPRING_MAGIC
    self.DAMPING_MAGIC = CONSTS.DAMPING_MAGIC
    self.MASS_CENTRE = Point3(CONSTS.MASS_CENTRE_X, CONSTS.MASS_CENTRE_Y, CONSTS.MASS_CENTRE_Z)
    self.REV_ALPHA = CONSTS.REV_ALPHA
    self.TURN_ALPHA = CONSTS.TURN_ALPHA
    self.DRAG_COEFF = CONSTS.DRAG_COEFF
    self.MAX_ENG_FORCE    = CONSTS.MAX_ENG_FORCE       
    self.MAX_BRAKE_FORCE  = CONSTS.MAX_BRAKE_FORCE  
    self.FRICTION_STATIC  = CONSTS.FRICTION_STATIC
    self.FRICTION_MAX     = CONSTS.FRICTION_MAX
    self.FRICTION_SLIDING = CONSTS.FRICTION_SLIDING
    self.SPRING_K = (self.MASS * 9.81) / (len(self.WHEELS) * self.DISPLACEMENT)
    self.DAMPING       = 2.0 * math.sqrt(self.SPRING_K * self.MASS)
    
    # self.physics = eRacer.TriMesh()    
    self.physics = eRacer.Box(
      True,       # dynamic
      self.MASS,  # mass
      self.INITIAL_POS, # position
      Matrix(),   # orientation
      self.SIZE   # bounds
    )
    self.transform = Matrix()
    
    self.graphics = scene.CreateMovingMeshNode("vehicle")
    self.graphics.thisown = 0

    self.physics.SetCentreOfMass(self.MASS_CENTRE)

    def load(r):
      if not r:
        # self.physics.Init(self.graphics.mesh());
        self.graphics.initialized = True
      else:
        print 'Failed to load mesh!'      
    
    game().io.LoadMeshAsync(load, self.graphics, self.MODEL)   
  
    self.throttle = 0.      # position of the throttle on game controller from 0 to 1
    self.brake    = False   # brake button
    self.steerPos = 0.      # position of the control stick from left to right ranges from -1 to +1

    self.acceleration = 0.  # physics engine setting.   from 0 to 1
    self.turning      = 0.  # physics steering setting. from 0 to 1
    
    self.sliding = [False] * len(self.WHEELS) # static vs sliding state of each wheel
    self.crashtime = 0      # time since wheels were last in contact with the ground
    
    game().event.Register(self.PlayerAccelerateEvent)
    game().event.Register(self.PlayerTurnEvent)
    game().event.Register(self.PlayerBrakeEvent)
    game().event.Register(self.ReloadedConstsEvent)
    
  # control events
  def PlayerBrakeEvent(self, brake):
    self.brake = brake

  def PlayerAccelerateEvent(self, changeThrottle):
    self.throttle = changeThrottle

  def PlayerTurnEvent(self, turn):
    self.steerPos = turn
  
  def Tick(self, time):
    Entity.Tick(self, time)
    
    if not time.game_delta:
      return
    
    phys  = self.physics
    tx    = phys.GetTransform()
    delta = float(time.game_delta) / time.RESOLUTION

    self.PrintDebug()
    # do engine/brake/steering/user input forces    
    
    alphaa = math.pow(self.REV_ALPHA,  delta)
    alphat = math.pow(self.TURN_ALPHA, delta)
    
    self.acceleration = (alphaa)*self.acceleration + (1-alphaa)*self.throttle
    self.turning      = (alphat)*self.turning      + (1-alphat)*self.steerPos

    # suspension axis (pointing down from car)
    axis  = mul0(tx, -Y)
    
    bodyVel = phys.GetVelocity();
    dragForceMag = self.DRAG_COEFF * length(bodyVel) ** 2
    dragDir = normalize(bodyVel) * -1.0
    dragForce = dragDir * dragForceMag
    phys.AddWorldForceAtLocalPos(dragForce, self.MASS_CENTRE)
    #print dragForce.x, dragForce.y, dragForce.z
    
    crashed = True
    for i,localpos in enumerate(self.WHEELS):
      # wheel vectors in world space
      worldpos   = mul1(tx, localpos)
      worldvel   = phys.GetLocalPointWorldVelocity(localpos)
      
      #raycast down from suspension point
      upamount = 1.0
      worldroadnormal = Vector3()
      localsuspoint   = Point3(localpos.x, localpos.y + upamount, localpos.z)
      worldsuspoint   = mul1(tx, localsuspoint)
      dist = phys.RaycastDown(worldsuspoint, worldroadnormal) - upamount
      disp = (self.DISPLACEMENT - dist)
      

      _debug = [self.DEBUG[i]]
      def debug(s):
        if not CONSTS.CAR_DEBUG: return
        game().graphics.graphics.WriteString(s, "Verdana", 12, _debug[0])
        _debug[0] = _debug[0] + Point3(0, 20, 0)
      
      debug("Disp: %6.3f" % disp)
      
      # check for invalid distances
      if disp < -100:
        # wheel is in the air - no forces applied, ignore it
        debug("AIR")
        # we should reset
        continue
      if -20 < disp < 0:
        #print "in the air after jump"
        crashed = False
        debug("JUMP")
        continue
      if disp > 3*self.DISPLACEMENT:
        # sanity check
        debug("BAD")
        continue
      crashed = False
      
      # spring force
      downforce = worldroadnormal * disp * self.SPRING_K * self.SPRING_MAGIC
      #print delta, disp, self.SPRING_K, self.SPRING_MAGIC
      #print force.x, force.y, force.z
      phys.AddWorldForceAtLocalPos(downforce, localpos)
      
      # shock absorber forces
      linearvel = -dot(worldvel, worldroadnormal)
      slowforce = worldroadnormal * linearvel * self.DAMPING * self.DAMPING_MAGIC
      phys.AddWorldForceAtLocalPos(slowforce, localpos)
      
      # do accelleration
      
      forwardSpeed = self.GetWheelSpeed(delta, downforce)
      #forwardSpeed = self.GetWheelSpeed(delta)
      debug("FW: %6.2f" % forwardSpeed)
      
      # TODO modify Z for steering
      # direction of the wheel on the surface of the road
      # front wheel turns
      
      angle = self.turning * min(1.,(60. / max(1.,length(worldvel))**1.4))
      debug("angle: %6.2f" % angle)
      if i < 2: turning = Matrix(ORIGIN, angle, Y)
      else:     turning = Matrix()
      
      # +Z is forward in the local space
      worldrollingdir = mul0(tx, mul0(turning, Z))
      worldrollingvel = worldrollingdir * dot(worldrollingdir, worldvel)
        
            
      worldforwardvel = mul0(tx, mul0(turning, Z * forwardSpeed))
      
      # wheel's current motion on the surface of the road
      worldwheelvel = worldvel - worldroadnormal * dot(worldvel, worldroadnormal)
      
      # difference of where the wheel wants to go, and where it is really going.

      powerforce = worldrollingvel + worldforwardvel - worldvel
      
      staticfrictionmax = self.FRICTION_SLIDING * length(downforce+slowforce)
      
      if self.sliding[i]:
        powerforce = powerforce * self.FRICTION_SLIDING * length(downforce)
        debug("SLIDING")
      else:
        debug("STATIC")
        powerforce = powerforce * self.FRICTION_STATIC * length(downforce)
      debug("(%6.2f %6.2f %6.2f)" % (powerforce.x, powerforce.y, powerforce.z))
      debug("Power:  %6.2f" % length(powerforce))
      debug("Static: %6.2f" % staticfrictionmax)
      
      phys.AddWorldForceAtLocalPos(powerforce, localpos)
      
    # no wheels are touching the ground.
    # reset the car
    if not crashed:
      self.crashtime = 0
    else:
      self.crashtime += delta
    
    if self.crashtime > 3: # or car stopped? ## why stopped?
      self.crashtime = 0
      print "Crash! resetting car"
      self.resetCar()

    self.transform = tx
    self.graphics.SetTransform(Matrix(ORIGIN, math.pi, Y) * tx)

  # def set_transform(self, transform):
  #   Entity.set_transform(self, transform)
  #   self.graphics.SetTransform(Matrix(ORIGIN, math.pi, Y) * transform)

  # transform = property(Entity.get_transform, set_transform)   
  
  def PrintDebug(self):
    if not CONSTS.CAR_DEBUG: return
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
  
  def GetWheelSpeed(self, timeStep, normalForce):
    gravityMag = 9.81 #should change based on track segment
    forceMag = self.MAX_ENG_FORCE * self.acceleration
    if self.physics.GetSpeed() < 1:
      brakeMag =0
    else:
      brakeMag = self.MAX_BRAKE_FORCE * self.brake
    massOnTire = length(normalForce) / gravityMag
    if forceMag > 0:
      speedDelta = (forceMag-brakeMag) / massOnTire
    else:
      speedDelta = (forceMag+brakeMag) / massOnTire
    return speedDelta
  
  #def GetWheelSpeed(self, timeStep):
   # forceMag = self.maxEngForce * self.acceleration
    #brakeMag = self.maxBrakeForce * self.brake * -1.0
    #speedDelta = (forceMag+brakeMag) * timeStep
    #return speedDelta
    
    
  def ReloadedConstsEvent(self):
    #can't set mass or centre of mass on the fly due to Physx
    #self.MASS = CONSTS.CAR_MASS
    #self.physics.SetMass(self.MASS) #setting the mass breaks the suspension for some reason
    #maybe physx is still running
    self.SPRING_MAGIC = CONSTS.SPRING_MAGIC
    self.DAMPING_MAGIC = CONSTS.DAMPING_MAGIC
    self.SPRING_K = (self.MASS * 9.81) / (len(self.WHEELS) * self.DISPLACEMENT)
    self.DAMPING       = 2.0 * math.sqrt(self.SPRING_K * self.MASS)

    #self.MASS_CENTRE = Point3(CONSTS.MASS_CENTRE_X, CONSTS.MASS_CENTRE_Y, CONSTS.MASS_CENTRE_Z)
    self.REV_ALPHA = CONSTS.REV_ALPHA
    self.TURN_ALPHA = CONSTS.TURN_ALPHA
    self.DRAG_COEFF = CONSTS.DRAG_COEFF
    self.MAX_ENG_FORCE    = CONSTS.MAX_ENG_FORCE       
    self.MAX_BRAKE_FORCE  = CONSTS.MAX_BRAKE_FORCE  
    self.FRICTION_STATIC  = CONSTS.FRICTION_STATIC
    self.FRICTION_MAX     = CONSTS.FRICTION_MAX
    self.FRICTION_SLIDING = CONSTS.FRICTION_SLIDING
    
  def resetCar(self):
      phys  = self.physics
      tx    = phys.GetTransform()
      normal = Y
      forward = mul0(tx, Z)
      forward = forward - normal * dot(normal, forward)
      pos = self.INITIAL_POS
      phys.SetVelocity(Vector3())
      phys.SetOrientation(Matrix())
      phys.SetAngVelocity(Vector3())
      phys.SetPosition(pos)
      # eRacer.ExtractPosition(tx, pos)
      # pos.y = 1.5
      #tx = Matrix(pos, math.atan2(forward.y, forward.x), Y)
      #phys.SetTransform(tx)
    
