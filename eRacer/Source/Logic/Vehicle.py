from Core.Globals import *

class Vehicle(Entity):
  MODEL   = "Ship_06.x"
  SIZE    = Vector3(2, 1, 4)
  WHEELS  = [ # location of wheels on object
    Point3(-1, 0,  2), # front left
    Point3( 1, 0,  2), # front right
    Point3(-1, 0, -2), # back left
    Point3( 1, 0, -2), # back right
  ]
  
  
    
  def __init__(self, game):
    Entity.__init__(self, game)
    
    self.physics = eRacer.Box(
      True,       # dynamic
      2000,       # mass
      Vector3(0, 2, 0), # position
      Matrix(),   # orientation
      self.SIZE   # bounds
    )
    
    self.graphics = game().graphics.scene.CreateMovingGeometry("vehicle")
    self.graphics.visible = False
        
    def load(r):
      if not r:
        self.graphics.visible = True
      else:
        print 'Failed to load mesh!'      
      
    game().io.LoadMeshAsync(load, self.graphics, self.MODEL)   
    
  
  def Tick(self, time):
    Entity.Tick(self, time)
    
    # do engine/brake/steering forces
    
    
    phys = self.physics
    tx = phys.GetTransform()
    self.graphics.SetTransform(tx)
    
    for wheel in self.WHEELS:
      # position of wheel in world space
      pos = mul1(tx, wheel)
      
      # cast a ray to the road, get distance
      # ...
      
      # do spring forces
      # ...
      phys.AddForce(Vector3(0, 0, 0), pos)
      
      # do shock absorber forces
      # ...
      phys.AddForce(Vector3(0, 0, 0), pos)
      
      
      

