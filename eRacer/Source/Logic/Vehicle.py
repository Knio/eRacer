from Core.Globals import *

class Vehicle(Entity):
  MODEL   = "Ship_06.x"
  MODEL   = "box2.x"
  SIZE    = Vector3(2, 1, 4) # "radius" (double for length)
  WHEELS  = [ # location of wheels on object
    Point3(-1, 0,  2), # front left
    Point3( 1, 0,  2), # front right
    Point3(-1, 0, -2), # back left
    Point3( 1, 0, -2), # back right
  ]
  MASS    = 2000
  THRUST  = 2e-4 * MASS
  TURN    = 3e-6
  
    
  def __init__(self, game):
    Entity.__init__(self, game)
    
    self.physics = eRacer.Box(
      True,       # dynamic
      self.MASS,  # mass
      Vector3(0, 2, 0), # position
      Matrix(),   # orientation
      self.SIZE   # bounds
    )
    
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
    
    phys = self.physics
    tx = phys.GetTransform()
    d = time.game_delta

    # do engine/brake/steering/user input forces
    if game().input[KEY.W]:
      # all wheel drive for now
      for wheel in self.WHEELS:
        print wheel
        phys.AddForce(Vector3(0, 0, 1)*self.THRUST*d, wheel)
    
    if game().input[KEY.S]:
      # all wheel drive for now
      for wheel in self.WHEELS:
        print wheel
        phys.AddForce(Vector3(0, 0,-1)*self.THRUST*d, wheel)
    
    if game().input[KEY.A]:
      # !!hack!!
      rot = Matrix(ORIGIN,-self.TURN*d, Y)
      tx  = rot * tx
      phys.SetTransform(tx)
      
    if game().input[KEY.D]:
      # !!hack!!
      rot = Matrix(ORIGIN, self.TURN*d, Y)
      tx  = rot * tx
      phys.SetTransform(tx)
    
    for wheel in self.WHEELS:
      # position of wheel in world space
      pos = mul1(tx, wheel)
      
      # cast a ray to the road, get distance
      # we don't have a road yet, so it is implicitly a plane at y=0
      d = pos.y
      
      
      # do spring forces
      # ...
      phys.AddForce(Vector3(0, 0, 0), pos)
      
      # do shock absorber forces
      # ...
      phys.AddForce(Vector3(0, 0, 0), pos)
      
    #tx = Matrix()
    self.graphics.SetTransform(tx)

