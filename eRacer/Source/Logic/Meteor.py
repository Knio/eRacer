import random

from Core.Globals import *

class MeteorManager(Entity):
  SPAWNING_DISTANCE = 500.

  MIN_FORCE = 10000.
  MAX_FORCE = 20000.

  # between 0 and 1 
  SCATTERING = 0.3
  
  MIN_SIZE = 1.
  MAX_SIZE = 10.

  # between 0 and the radius of the meteor
  TUMBLING = 0.9
  
  MODEL = "leather-box.x"


  def __init__(self, scene):
    Entity.__init__(self)
    self.scene = scene 
    self.meteors = []
  
  
  def spawn(self, pos, scale, forceDir, forceMag, tumbling):
    meteor = Meteor(self.scene, self.MODEL,scale)
    #needs improvement - probably better angular velocity
    forcePos = Point3(tumbling,tumbling,tumbling)
    meteor.respawn(pos,forceDir, forceMag, forcePos)
    return meteor
    
  def spawnRandom(self):
    scale = random.uniform(self.MIN_SIZE, self.MAX_SIZE)

    meteor = Meteor(self.scene, "leather-box.x",scale)
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
    mag = u(self.MIN_FORCE, self.MAX_FORCE)*meteor.scale

    direc.x *= -1.+u(-self.SCATTERING, self.SCATTERING)
    direc.y *= -1.+u(-self.SCATTERING, self.SCATTERING)
    direc.z *= -1.+u(-self.SCATTERING, self.SCATTERING)
    
    forcePos = Point3()
    forcePos.x = u(-self.TUMBLING, self.TUMBLING)
    forcePos.x = u(-self.TUMBLING, self.TUMBLING)
    forcePos.x = u(-self.TUMBLING, self.TUMBLING)
    
    
    meteor.respawn(pos,normalized(direc), mag, forcePos)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    
    for meteor in self.meteors:
      if(length(eRacer.ExtractPosition(meteor.transform))>self.SPAWNING_DISTANCE):
        self.respawnRandom(meteor) 

class Meteor(Entity):
  def __init__(self, scene, model, scale=1):
    Entity.__init__(self)

    #hack: scale has to be stored separatly because physiscs will keep overriding it 
    self.scale = scale
    print self.scale
    self.transform = eRacer.CreateMatrix()

    self.physics = eRacer.Box(True, 4, eRacer.ORIGIN, eRacer.IDENTITY, Vector3(scale,scale,scale))
    self.graphics = scene.CreateMovingMeshNode("Meteor")
    self.graphics.thisown = 0
        
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      
    game().io.LoadMeshAsync(load, self.graphics, model)   
    
  def respawn(self, pos, forceDir, forceMag, forcePos):
    #scaling does not work because physics ignores it.
    self.transform = eRacer.CreateMatrix(pos)
    self.physics.SetTransform(self.transform)
    
    force = forceDir * forceMag
    
    self.physics.AddWorldForceAtLocalPos(force, forcePos)    
    
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()
    
    
    self.graphics.SetTransform(eRacer.Scaled(self.transform, self.scale,self.scale,self.scale))  
        