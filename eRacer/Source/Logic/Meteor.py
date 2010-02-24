import random

from Core.Globals import *

class Meteor(Entity):
  SPAWNING_DISTANCE = 200.
  BASE_FORCE = 5000.
  VARIABLE_FORCE = 1000.
  
  # between 0 and 1 
  SCATTERING = 0.3
  
  # between 0 and the radius of the meteor
  TUMBLING = 0.2
    
  def __init__(self, scene, model):
    Entity.__init__(self)

    self.transform = Matrix()

    self.physics = eRacer.Box(True, 4)
    self.graphics = scene.CreateMovingMeshNode("Meteor")
    self.graphics.thisown = 0
        
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.respawn();
      
    game().io.LoadMeshAsync(load, self.graphics, model)   
    
  def respawn(self):
    r = random.random
    # the meteor is spawned in a random location on a sphere
    direction = normalized(Vector3(r()-0.5,r()-0.5,r()-0.5))
    
    pos = direction * self.SPAWNING_DISTANCE
    mag = self.BASE_FORCE + r()*self.VARIABLE_FORCE

    # needs scattering
    force = direction * -mag
    force.x *= random.uniform(1.0-self.SCATTERING, 1.0+self.SCATTERING)
    force.y *= random.uniform(1.0-self.SCATTERING, 1.0+self.SCATTERING)
    force.z *= random.uniform(1.0-self.SCATTERING, 1.0+self.SCATTERING)
    
    forcePos = Point3(r(),r(),r())*self.TUMBLING
    
    self.physics.SetPosition(pos)
    self.physics.AddWorldForceAtLocalPos(force, forcePos)    
    
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()
    self.graphics.SetTransform(self.transform)  
    
    if(length(eRacer.ExtractPosition(self.transform))>self.SPAWNING_DISTANCE):
      self.respawn()
    