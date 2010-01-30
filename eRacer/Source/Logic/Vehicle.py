from Core.Globals import *

class Vehicle(Entity):
  MODEL   = "Ship_06.x"
  MODEL   = "box2.x"
  SIZE    = Vector3(1, .5, 2) # "radius" (double for length)
  WHEELS  = [ # location of wheels on object
    Point3(-1, -1.0,  2), # front left
    Point3( 1, -1.0,  2), # front right
    Point3(-1, -1.0, -2), # back left
    Point3( 1, -1.0, -2), # back right
  ]
  MASS    = 2000.0
  THRUST  = 5.0e+2 * MASS 
  TURN    = 3.0e+0          
  DISPLACEMENT = 0.05  # from wheel rest position
  
  # MASS * G = 4 * K * DISPLACEMENT
  SPRING_K = (MASS * 9.81) / (len(WHEELS) * DISPLACEMENT)
  SPRING_MAGIC  = 1.0 # tuning parameter
  
  DAMPING       = 2.0 * math.sqrt(SPRING_K * MASS)
  DAMPING_MAGIC = 1.0 # tuning parameter
  
    
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

    #eRacer.debug(tx)

    # hack hack hack hack hack
    # do engine/brake/steering/user input forces
    if game().input[KEY.W]:
      # all wheel drive for now
      for wheel in self.WHEELS:
        phys.AddLocalForceAtLocalPos(Vector3(0, 0, 1) * delta * self.THRUST, wheel)
    
    if game().input[KEY.S]:
      # all wheel drive for now
      for wheel in self.WHEELS:
        phys.AddLocalForceAtLocalPos(Vector3(0, 0,-1) * delta * self.THRUST, wheel)
    
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
    
    for wheel in self.WHEELS:
      # position of wheel in world space
      pos = mul1(tx, wheel)

      # we don't have a road yet, so it is implicitly a plane at y=0
      
      # road normal - assume +Y      
      normal = Vector3(0,1,0)
      
      # cast a ray to the road, get distance
      dist = pos.y
      #print dist  
      disp = (self.DISPLACEMENT - dist)
      if disp < 0:
        disp = 0 # car is in the air - no force from wheels
        
      
      # spring force
      force = +normal * disp * self.SPRING_K * self.SPRING_MAGIC
      print delta, disp, self.SPRING_K, self.SPRING_MAGIC
      print force.x, force.y, force.z
      phys.AddWorldForceAtLocalPos(force, wheel)
      
      # do shock absorber forces
      #vel = phys.GetPointVelocity(wheel)
      #linearvel = dot(vel, normal)
      #phys.AddWorldForceAtLocalPos(-normal * delta * linearvel * self.DAMPING * self.DAMPING_MAGIC, wheel)
      
      
    #tx = Matrix()
    self.graphics.SetTransform(tx)

