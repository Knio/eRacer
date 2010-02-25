import random

from Core.Globals import *

class MeteorManager(Entity):
  SPAWNING_DISTANCE = 200.

  MIN_FORCE = 10000.
  MAX_FORCE = 50000.

  # between 0 and 1 
  SCATTERING = 0.3
  
  MIN_SIZE = 0.1
  MAX_SIZE = 100.

  # between 0 and the radius of the meteor
  TUMBLING = 0.2
  
  MODEL = "leather-box.x"


  def __init__(self, scene):
    Entity.__init__(self)
    self.scene = scene 
    self.meteors = []
  
  
  def spawn(self, pos, scale, forceDir, forceMag, tumbling):
    meteor = Meteor(self.scene, self.MODEL)
    #needs improvement - probably better angular velocity
    forcePos = Point3(tumbling,tumbling,tumbling)
    meteor.respawn(pos,scale, forceDir, forceMag, forcePos)
    return meteor
    
  def spawnRandom(self):
    meteor = Meteor(self.scene, "leather-box.x")
    self.meteors.append(meteor)
    self.respawnRandom(meteor)
    return meteor
            
  def spawnAimed(self, aim, dir, force, tumbling):
    pass
    
  def respawnRandom(self, meteor):
    r = random.random
    u = random.uniform
    # the meteor is spawned in a random location on a sphere
    direc = normalized(Vector3(u(-1.,1.),u(-1.,1.),u(-1.,1.)))
    
    pos = direc * self.SPAWNING_DISTANCE
    mag = u(self.MIN_FORCE, self.MAX_FORCE)
    print mag

    direc.x *= -1.+u(-self.SCATTERING, self.SCATTERING)
    direc.y *= -1.+u(-self.SCATTERING, self.SCATTERING)
    direc.z *= -1.+u(-self.SCATTERING, self.SCATTERING)
    
    forcePos = Point3()
    forcePos.x = u(-self.TUMBLING, self.TUMBLING)
    forcePos.x = u(-self.TUMBLING, self.TUMBLING)
    forcePos.x = u(-self.TUMBLING, self.TUMBLING)
    
    scale = u(self.MIN_SIZE, self.MAX_SIZE)
    
    meteor.respawn(pos,scale, normalized(direc), mag, forcePos)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    
    for meteor in self.meteors:
      if(length(eRacer.ExtractPosition(meteor.transform))>self.SPAWNING_DISTANCE):
        self.respawnRandom(meteor) 

class Meteor(Entity):
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
      
    game().io.LoadMeshAsync(load, self.graphics, model)   
    
  def respawn(self, pos, scale, forceDir, forceMag, forcePos):
    #scaling does not work because physics ignores it.
    self.transform = eRacer.CreateMatrix(pos,scale)
    self.physics.SetTransform(self.transform)
    
    force = forceDir * forceMag
    
    self.physics.AddWorldForceAtLocalPos(force, forcePos)    
    
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()
    self.graphics.SetTransform(self.transform)  
        