from Core.Globals import *

class Vehicle(Entity):
  
  MODEL   = "Ship1.x"
  #MODEL   = "box.x"
  SIZE    = Vector3(2, 1, 4) # "radius" (double for length)
  WHEELS  = [ # location of wheels in object space
    Point3(-2, -1.5,  4), # front left
    Point3( 2, -1.5,  4), # front right
    Point3(-2, -1.5, -4), # back left
    Point3( 2, -1.5, -4), # back right
  ]
  MASS    = 5000.0
  THRUST  = 1.0e1 * MASS
  TURN    = 3.0e+0          
  DISPLACEMENT = 0.30  # from wheel rest position
  
  # MASS * G = 4 * K * DISPLACEMENT
  # TODO: get G from CONSTS
  SPRING_K = (MASS * 9.81) / (len(WHEELS) * DISPLACEMENT)
  SPRING_MAGIC  = 1.0 # tuning parameter
  
  DAMPING       = 2.0 * math.sqrt(SPRING_K * MASS)
  DAMPING_MAGIC = 1.0 # tuning parameter
  
  STATIC_FRICTION   = 1.0
  SLIDING_FRICTION  = 0.5
  
  MAX_SPEED = 40.0
  
  REV_ALPHA   = 1.0/8.0
  TURN_ALPHA  = 1.0/5.0
  
  
  def __init__(self, scene):
    Entity.__init__(self)
    
    self.physics = eRacer.Box(
      True,       # dynamic
      self.MASS,  # mass
      Vector3(80, 3, 0), # position
      Matrix(),   # orientation
      self.SIZE   # bounds
    )
    
    self.graphics = scene.CreateMovingGeometry("vehicle")
    self.physics.SetCentreOfMass(Point3(0, -1, 0))

    def load(r):
      if not r:
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
    
    self.maxEngForce    = 5e4   #the max amount the engine can put on a wheel at the moment
                                #constant for now, will be variable later
    self.maxBrakeForce  = 5e4   #always constant
    
    game().event.Register(self.PlayerAccelerateEvent)
    game().event.Register(self.PlayerTurnEvent)
    game().event.Register(self.PlayerBrakeEvent)
    
    
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
    
    crashed = True
    ddd = []
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
      
      # check for invalid distances
      if disp < 0:
        # whee is in the air - no it will not have any forcesww
        ddd.append(-1)
        continue
      if disp > 3*self.DISPLACEMENT:
        ddd.append(-2)
        # sanity check
        continue
      ddd.append(disp)
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
      # print forwardSpeed
      
      # TODO modify Z for steering
      # direction of the wheel on the surface of the road
      # front wheel turns
      if i < 2: turning = Matrix(ORIGIN, self.turning, Y)
      else:     turning = Matrix()
      
      # +Z is forward in the local space
      worldrollingdir = mul0(tx, mul0(turning, Z))
      worldrollingvel = worldrollingdir * dot(worldrollingdir, worldvel)
        
            
      worldforwardvel = mul0(tx, mul0(turning, Z * forwardSpeed))
      
      # wheel's current motion on the surface of the road
      worldwheelvel = worldvel - worldroadnormal * dot(worldvel, worldroadnormal)
      

      powerforce = worldrollingvel + worldforwardvel - worldvel
      

      
      
      if self.sliding[i]:
        powerforce = powerforce * self.SLIDING_FRICTION * length(downforce)
      else:
        powerforce = powerforce * self.STATIC_FRICTION  * length(downforce)
        
       # print powerforce.x, powerforce.y, powerforce.z
      
      phys.AddWorldForceAtLocalPos(powerforce, localpos)
      
    # no wheels are touching the ground.
    # reset the car
    if not crashed:
      self.crashtime = 0
    else:
      self.crashtime += delta
      
    if self.crashtime > 2: # or car stopped?
      self.crashtime = 0
      print "Crash! resetting car"
      normal = Y
      forward = mul0(tx, Z)
      forward = forward - normal * dot(normal, forward)
      pos = Point3()
      eRacer.ExtractPosition(tx, pos)
      pos.y = 1.5
      tx = Matrix(pos, math.atan2(forward.y, forward.x), Y)
      phys.SetTransform(tx)
      
    # print ''.join('%6.2f' % i for i in ddd),
    # print self.acceleration, self.turning
    
    #tx = Matrix()
    self.transform = tx

  def set_transform(self, transform):
    Entity.set_transform(self, transform)
    self.graphics.SetTransform(Matrix(ORIGIN, math.pi, Y) * transform)

  transform = property(Entity.get_transform, set_transform)   
  
  def PrintDebug(self):
    # print debug info
    phys = self.physics
    velx = phys.GetVelocity().x
    vely = phys.GetVelocity().y
    velz = phys.GetVelocity().z
    velStr = "Vel: %2.3f" %velx + " %2.3f " %vely + " %2.3f " %velz
    game().graphics.graphics.WriteString(
      velStr, "Verdana", 24, Point3(0,0,5))
      
    speed = phys.GetSpeed()
    speedStr = "Speed: %2.3f"%speed
    game().graphics.graphics.WriteString(
      speedStr, "Verdana", 24, Point3(0,50,500))
    
    throttleStr = "Throttle:  %1.3f"%self.throttle
    game().graphics.graphics.WriteString(
      throttleStr, "Verdana", 24, Point3(0,100,0))
      
    steerStr = "Control L/R:  %1.3f"%self.steerPos
    game().graphics.graphics.WriteString(
      steerStr, "Verdana", 24, Point3(0,150,0))
    
  #get the speed the car wants to add to this wheel in the forward direction
  #this will be due to the braking or acceleration the user wants
  #needs the weight on this tire
  #returns a floating number, if negative, then a braking force was applied
  def GetWheelSpeed(self, timeStep, normalForce):
    gravityMag = 9.81 #should change based on track segment
    forceMag = self.maxEngForce * self.acceleration
    brakeMag = self.maxBrakeForce * self.brake * -1.0
    massOnTire = length(normalForce) / gravityMag
    speedDelta = (forceMag+brakeMag) / massOnTire * timeStep
    return speedDelta
    
    
