import random

from Core.Globals import *

class MeteorManager(Entity):
  VANISHING_DISTANCE = 1500.

  def __init__(self, scene):
    Entity.__init__(self)
    self.scene = scene 
    self.meteors = []
    game().event.Register(self.MeteorMeteorCollisionEvent)
    game().event.Register(self.MeteorCarCollisionEvent)
    game().event.Register(self.MeteorTrackCollisionEvent)

  
  def spawn(self, pos, scale, forceDir, forceMag, tumbling):
    pass
    # meteor = Meteor(self.scene, self.MODELS[random.randrange(len(self.MODELS))],scale)
    # #needs improvement - probably better angular velocity
    # forcePos = Point3(tumbling,tumbling,tumbling)
    # meteor.respawn(pos,forceDir, forceMag, forcePos)
    # return meteor
    
  def spawnRandom(self):
    m = RandomMeteor(self.scene)
    self.meteors.append(m)
    return m
            
  def spawnAimed(self, aim):
    m = AimedMeteor(self.scene,aim)
    self.meteors.append(m)
    return m

    # u = random.uniform
    # forceDir = Vector3(0,-1,0)
    # forceDir.x *= 1.+u(-self.AIMED_SCATTERING, self.AIMED_SCATTERING)
    # forceDir.y *= 1.+u(-self.AIMED_SCATTERING, self.AIMED_SCATTERING)
    # forceDir.z *= 1.+u(-self.AIMED_SCATTERING, self.AIMED_SCATTERING)
    
    # pos = aim - forceDir * self.SPAWNING_DISTANCE

    # scale = random.uniform(self.MIN_SIZE, self.MAX_SIZE)

    # meteor = Meteor(self.scene, self.MODELS[random.randrange(len(self.MODELS))], scale)
    # self.meteors.append(meteor)
    
    # forceMag = random.uniform(self.AIMED_MIN_FORCE, self.AIMED_MAX_FORCE)
    
    # meteor.respawn(pos,normalized(forceDir), forceMag, self.randomForcePosition())
    # return meteor
    
    
  # def respawnRandom(self, meteor):

  #   r = random.random
  #   u = random.uniform
  #   # the meteor is spawned in a random location on a sphere
  #   direc = normalized(Vector3(u(-1.,1.),u(-1.,1.),u(-1.,1.)))
    
  #   pos = direc * self.SPAWNING_DISTANCE

  #   direc.x *= -1.+u(-self.SCATTERING, self.SCATTERING)
  #   direc.y *= -1.+u(-self.SCATTERING, self.SCATTERING)
  #   direc.z *= -1.+u(-self.SCATTERING, self.SCATTERING)
        
  #   forcePos = self.randomForcePosition()
   
  #   meteor.respawn(pos,normalized(direc), self.randomForceMagnitude(), forcePos)
  
  # def randomForcePosition(self):
  #   u = random.uniform
  #   forcePos = Point3()
  #   forcePos.x = u(-self.TUMBLING, self.TUMBLING)
  #   forcePos.y = u(-self.TUMBLING, self.TUMBLING)
  #   forcePos.z = u(-self.TUMBLING, self.TUMBLING)
  #   return forcePos
    
  # def randomForceMagnitude(self):
  #   u = random.uniform
  #   return u(self.MIN_FORCE, self.MAX_FORCE)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    
    for meteor in self.meteors:
      pos = eRacer.ExtractPosition(meteor.transform)
      if(length(pos)>self.VANISHING_DISTANCE):
        #print "respawning at position ",pos," with distance ",length(pos)," from origin." 
        if(not meteor.reset()):
          self.meteors.remove(meteor)
          
        
        
  def MeteorMeteorCollisionEvent(self, pair):
    print "MM Collision reported to MeteorManager"

  def MeteorTrackCollisionEvent(self, pair):
    print "MT Collision reported to MeteorManager"

  def MeteorCarCollisionEvent(self, pair):
    print "MC Collision reported to MeteorManager"


class Meteor(Entity):
  MASS              = 3000.
  MODELS = ["Meteor1.x", "Meteor2.x"]

  
    
  def __init__(self, scene, scale=1):
    Entity.__init__(self)

    #hack: scale has to be stored separately because physiscs will keep overriding it 
    self.scale = scale

    self.transform = eRacer.CreateMatrix()

    self.physics = eRacer.Box(True, self.MASS, eRacer.ORIGIN, eRacer.IDENTITY, Vector3(scale,scale,scale))
    self.physics.SetGroup(eRacer.METEOR)
    self.graphics = scene.CreateMovingMeshNode("Meteor")
    self.graphics.thisown = 0
        
        
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      
    game().io.LoadMeshAsync(load, self.graphics, self.MODELS[random.randrange(len(self.MODELS))])   
    
  def reset(self):
    return True

    
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()    
    self.graphics.SetTransform(eRacer.Scaled(self.transform, self.scale,self.scale,self.scale))  

  def randomDirection():
    u = random.uniform
    return normalized(Vector3(u(-1.,1.),u(-1.,1.),u(-1.,1.)))

class RandomMeteor(Meteor):
  SPAWNING_DISTANCE = 1500.
  
  # between 0 and 1 
  SCATTERING = 0.3
  
  MIN_SPEED = 50
  MIN_SPEED = 200
  MIN_SIZE = 0.2
  MAX_SIZE = 10.

  # between 0 and the radius of the meteor
  TUMBLING = 0.9
  
  def __init__(self, scene):
    Meteor.__init__(self,scene, u(MIN_SIZE,MAX_SIZE))
    
    self.reset()
    
    
  def reset(self):
    r = random.random
    u = random.uniform
    
    # the meteor is spawned in a random location on a sphere
    direction = self.randomDirection()
    position = direction * self.SPAWNING_DISTANCE
    
    velocity = direction * -u(MIN_SPEED, MAX_SPEED)

    velocity.x *= 1.+u(-self.SCATTERING, self.SCATTERING)
    velocity.y *= 1.+u(-self.SCATTERING, self.SCATTERING)
    velocity.z *= 1.+u(-self.SCATTERING, self.SCATTERING)
    
    angular_velocity = self.TUMBLING*self.randomDirection()
   
    self.physics.SetTransform(CreateMatrix(position))
    self.physics.SetVelocity(velocity)
    self.physics.SetAngVelocity(angular_velocity)  
    return True  
  
class AimedMeteor(Meteor):
  #in seconds
  AIR_TIME = 3.
  
  
  # between 0 and 1 
  SCATTERING = 0.3
  
  MIN_SPEED = 150
  MIN_SPEED = 200
  MIN_SIZE = 0.2
  MAX_SIZE = 1.

  # between 0 and the radius of the meteor
  TUMBLING = 0.3
  
  def __init__(self,scene,aim):
    u = random.uniform
    Meteor.__init__(self,scene,u(self.MIN_SIZE, self.MAX_SIZE))
    
    direction = Vector3(0,-1,0)
    direction.x *= 1.+u(-self.SCATTERING, self.SCATTERING)
    direction.y *= 1.+u(-self.SCATTERING, self.SCATTERING)
    direction.z *= 1.+u(-self.SCATTERING, self.SCATTERING)

    velocity = direction * u(self.MIN_SPEED, self.MAX_SPEED)
    
    position = aim - velocity * self.AIR_TIME

    angular_velocity = self.TUMBLING*self.randomDirection()    
    
    forceMag = random.uniform(self.AIMED_MIN_FORCE, self.AIMED_MAX_FORCE)
    
    self.physics.SetTransform(CreateMatrix(position))
    self.physics.SetVelocity(velocity)
    self.physics.SetAngVelocity(angular_velocity)    
  
  def reset(self):
    return False
