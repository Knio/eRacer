from Core.Globals import *

class Vehicle(Entity):
  MODEL   = "Ship_06.x"
  MODEL   = "box2.x"
  SIZE    = Vector3(2, 1, 4) # "radius" (double for length)
  WHEELS  = [ # location of wheels on object
    Point3(-2, -1.5,  4), # front left
    Point3( 2, -1.5,  4), # front right
    Point3(-2, -1.5, -4), # back left
    Point3( 2, -1.5, -4), # back right
  ]
  MASS    = 5000.0
  THRUST  = 1.0e1 * MASS
  TURN    = 3.0e+0          
  DISPLACEMENT = 0.25  # from wheel rest position
  
  # MASS * G = 4 * K * DISPLACEMENT
  SPRING_K = (MASS * 9.81) / (len(WHEELS) * DISPLACEMENT)
  SPRING_MAGIC  = 1.0 # tuning parameter
  
  DAMPING       = 2.0 * math.sqrt(SPRING_K * MASS)
  DAMPING_MAGIC = 1.5 # tuning parameter
  
    
  def __init__(self, game):
    Entity.__init__(self, game)
    
    self.physics = eRacer.Box(
      True,       # dynamic
      self.MASS,  # mass
      Vector3(0, 2, 0), # position
      Matrix(),   # orientation
      self.SIZE   # bounds
    )
    print self.physics.GetMass()
    
    
    self.graphics = game.graphics.scene.CreateMovingGeometry("vehicle")
    self.graphics.visible = False
        
    def load(r):
      if not r:
        self.graphics.visible = True
      else:
        print 'Failed to load mesh!'      
      
    game.io.LoadMeshAsync(load, self.graphics, self.MODEL)   
    
    
  
  def Tick(self, time):
    Entity.Tick(self, time)
    
    phys  = self.physics
    tx    = phys.GetTransform()
    delta = float(time.game_delta) / time.RESOLUTION
    
    # hack hack hack hack hack
    # do engine/brake/steering/user input forces
    if game().input[KEY.W]:
      # all wheel drive for now
      for wheel in self.WHEELS:
        phys.AddLocalForceAtLocalPos(Vector3(0, 0, 1) * self.THRUST, wheel)
    
    if game().input[KEY.S]:
      # all wheel drive for now
      for wheel in self.WHEELS:
        phys.AddLocalForceAtLocalPos(Vector3(0, 0,-1) * self.THRUST, wheel)
    
    if game().input[KEY.A]:
      # !!hack!!
      rot = Matrix(ORIGIN, delta * -self.TURN, Y)
      tx  = rot * tx
      phys.SetTransform(tx)
      
    if game().input[KEY.D]:
      # !!hack!!
      rot = Matrix(ORIGIN, delta * self.TURN, Y)
      tx  = rot * tx
      phys.SetTransform(tx)
    
    
    #self.transform = tx
    #return
    #eRacer.debug(tx)
    ddd = []
    for wheel in self.WHEELS:
      # position of wheel in world space
      pos   = mul1(tx, wheel)
      
      # suspension axis (pointing down from car)
      axis  = mul0(tx, -Y)
      
      # we don't have a road yet, so it is implicitly a plane at y=0
      # road normal - assume +Y      
      normal = Vector3(0,1,0)
      
      # cast a ray to the road, get distance
      dist = pos.y / -dot(axis, normal)
      #print dist  
      disp = (self.DISPLACEMENT - dist)
      if disp < 0:
        # whee is in the air - no it will not have any forces
        ddd.append(-1)
        continue
      if disp > 3*self.DISPLACEMENT:
        ddd.append(-2)
        # sanity check
        continue
      ddd.append(disp)
      
      # spring force
      force = +normal * disp * self.SPRING_K * self.SPRING_MAGIC
      #print delta, disp, self.SPRING_K, self.SPRING_MAGIC
      #print force.x, force.y, force.z
      phys.AddWorldForceAtLocalPos(force, wheel)
      
      # do shock absorber forces
      vel = phys.GetPointVelocity(wheel)
      linearvel = -dot(vel, normal)
      force = normal * linearvel * self.DAMPING * self.DAMPING_MAGIC
      phys.AddWorldForceAtLocalPos(force, wheel)
      
    print ''.join('%6.2f' % i for i in ddd)
    
    #tx = Matrix()
    self.transform = tx

  def set_transform(self, transform):
    Entity.set_transform(self, transform)
    self.graphics.SetTransform(self.transform)  

  transform = property(Entity.get_transform, set_transform)   
