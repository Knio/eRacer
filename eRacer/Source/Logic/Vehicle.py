from Core.Globals import *

class Vehicle(Entity):
  MODEL   = "Ship1.x"
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
  
  REV_ALPHA   = 3.0/5.0
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
    self.graphics.visible = False
    self.physics.SetCentreOfMass(Point3(0, -1, 0))

    def load(r):
      if not r:
        self.graphics.visible = True
      else:
        print 'Failed to load mesh!'      
      
    game().io.LoadMeshAsync(load, self.graphics, self.MODEL)   
  
    self.acceleration = 0.
    self.turning      = 0.

    self.throttle = 0.00 # position of the throttle on game controller from -1 to +1
    #if < 0, might want different logic for braking
    self.steerPos = 0.00 # position of the control stick from left to right ranges from -1 to +1

    self.sliding = [False] * len(self.WHEELS)
    self.crashtime = 0
    
    game().event.Register(self.PlayerAccelerateEvent)
    game().event.Register(self.PlayerTurnEvent)
  #change in throttle ranges between -1 and +1
  def PlayerAccelerateEvent(self, changeThrottle):
    self.throttle += changeThrottle

  def PlayerTurnEvent(self, turn):
    self.steerPos += turn  

  
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
    
    crashed = True
    ddd = []
    for i,wheel in enumerate(self.WHEELS):
      # position of wheel in world space
      pos   = mul1(tx, wheel)
      
      # suspension axis (pointing down from car)
      axis  = mul0(tx, -Y)
      
      #raycast down
      normal = Vector3()
      localSusPoint = Point3(wheel.x, wheel.y + 0.5, wheel.z)
      susPoint = mul1(tx, localSusPoint)       
      dist = phys.RaycastDown(susPoint, normal) - 0.5
      disp = (self.DISPLACEMENT - dist)
 
      # we don't have a road yet, so it is implicitly a plane at y=0
      # road normal - assume +Y      
      #normal = Vector3(0,1,0)
      
      # cast a ray to the road, get distance
      # dist = pos.y / -dot(axis, normal)

      #print dist
      disp = (self.DISPLACEMENT - dist)
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
      downforce = normal * disp * self.SPRING_K * self.SPRING_MAGIC
      #print delta, disp, self.SPRING_K, self.SPRING_MAGIC
      #print force.x, force.y, force.z
      phys.AddWorldForceAtLocalPos(downforce, wheel)
      
      # do shock absorber forces
      vel = phys.GetPointVelocity(wheel)
      linearvel = -dot(vel, normal)
      slowforce = normal * linearvel * self.DAMPING * self.DAMPING_MAGIC
      phys.AddWorldForceAtLocalPos(slowforce, wheel)
      
      # do accelleration
      
      # TODO modify Z for steering
      # direction of the wheel on the surface of the road
      if i < 2: # front wheel
        turning = Matrix(ORIGIN, self.turning, Y)
      else:
        turning = Matrix()
      
      forward = mul0(tx, mul0(turning, Z * self.acceleration * self.MAX_SPEED))
      forward = forward - normal * dot(forward, normal)
      
      # wheel's motion on the surface of the road
      motion = vel - normal * dot(vel, normal)
      
      powerforce = forward - motion
      
      if self.sliding[i]:
        powerforce = powerforce * self.SLIDING_FRICTION * length(downforce)
      else:
        powerforce = powerforce * self.STATIC_FRICTION  * length(downforce)
        
      
      phys.AddWorldForceAtLocalPos(powerforce, wheel)
      
    # no wheels are touching the ground.
    # reset the car
    if not crashed:
      self.crashtime = 0
    else:
      self.crashtime += delta
      
    if self.crashtime > 2: # or car stopped?
      self.crashtime = 0
      print "Crash! resetting car"
      forward = mul0(tx, Z)
      forward = forward - normal * dot(normal, forward)
      pos = Point3()
      eRacer.ExtractPosition(tx, pos)
      pos.y = 1.5
      tx = Matrix(pos, math.atan2(forward.y, forward.x), Y)
      phys.SetTransform(tx)
      
    #print ''.join('%6.2f' % i for i in ddd),
    #print self.acceleration, self.turning
    
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
    
