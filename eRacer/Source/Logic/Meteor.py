import random
import math

from Core.Globals import *

class MeteorManager(Entity):
  VANISHING_DISTANCE = 600.

  def __init__(self, scene):
    Entity.__init__(self)
    self.scene = scene 
    self.meteors = []
    game().event.Register(self.MeteorMeteorCollisionEvent)
    game().event.Register(self.MeteorCarCollisionEvent)
    game().event.Register(self.MeteorTrackCollisionEvent)
    
    
  def spawnRandom(self):
    m = RandomMeteor(self.scene)
    self.meteors.append(m)
    return m
            
  def spawnTargeted(self, target):
    m = TargetedMeteor(self.scene,target)
    self.meteors.append(m)
    return m


  def spawnAimed(self, aim):
    m = AimedMeteor(self.scene,aim)
    self.meteors.append(m)
    return m

    
  def Tick(self, time):
    Entity.Tick(self, time)
    
    for meteor in self.meteors:
      pos = eRacer.ExtractPosition(meteor.transform)
      if(length(pos)>self.VANISHING_DISTANCE):
        #print "respawning at position ",pos," with distance ",length(pos)," from origin." 
        if(not meteor.reset()):
          self.meteors.remove(meteor)
    
      
        
        
  def MeteorMeteorCollisionEvent(self, meteorId1, meteorId2, force):
    pass
    #print "MM Collision reported to MeteorManager"

  def MeteorTrackCollisionEvent(self, meteorId, trackId, force):
    Entity.entities[meteorId].hitTrack(force)

  def MeteorCarCollisionEvent(self, meteorId, carId, force):
    pass
    #print "MC Collision reported to MeteorManager"


class Meteor(Entity):
  DENSITY   = 100.
  MODELS    = ["Meteor1.x", "Meteor2.x"]

  
    
  def __init__(self, scene, scale=1):
    Entity.__init__(self)

    #hack: scale has to be stored separately because physiscs will keep overriding it 
    self.scale = scale

    self.transform = eRacer.CreateMatrix()

    self.physics = eRacer.Box(True, self.DENSITY*self.scale*self.scale*self.scale, eRacer.ORIGIN, eRacer.IDENTITY, Vector3(scale,scale,scale))
    self.physics.SetGroup(eRacer.METEOR)
    self.physics.SetId(self.id)
    
    self.graphics = scene.CreateMovingMeshNode("Meteor")
    self.graphics.thisown = 0
        
        
    def load(mesh):
      if mesh:
        self.graphics.Init(mesh)    
      
    game().io.LoadMeshAsync(load, self.MODELS[random.randrange(len(self.MODELS))])   
    
  def reset(self):
    return True
    
  def hitTrack(self,force):
    if(self.scale<2.5):
      self.physics.PutToSleep()
      direction = normalized(force)
      pos = self.physics.GetPosition()-direction*self.scale
      
      self.physics.SetPosition(pos)
        
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()    
    self.graphics.SetTransform(eRacer.Scaled(self.transform, self.scale,self.scale,self.scale))  

  def randomDirection(self):
    u = random.uniform
    return normalized(Vector3(u(-1.,1.),u(-1.,1.),u(-1.,1.)))

class RandomMeteor(Meteor):
  SPAWNING_DISTANCE = 600.
  
  # between 0 and 1 
  SCATTERING = 0.3
  
  MIN_SPEED = 300
  MAX_SPEED = 500
  MIN_SIZE = .5
  MAX_SIZE = 5.

  # between 0 and the radius of the meteor
  TUMBLING = 0.9
  
  def __init__(self, scene):
    Meteor.__init__(self,scene, random.uniform(self.MIN_SIZE,self.MAX_SIZE))
    
    self.reset()
    
    
  def reset(self):
    u = random.uniform
    
    # the meteor is spawned in a random location on a sphere
    direction = self.randomDirection()
    position = direction * self.SPAWNING_DISTANCE
    
    velocity = direction * -(u(self.MIN_SPEED, self.MAX_SPEED)/math.sqrt(self.scale))

    velocity.x *= 1.+u(-self.SCATTERING, self.SCATTERING)
    velocity.y *= 1.+u(-self.SCATTERING, self.SCATTERING)
    velocity.z *= 1.+u(-self.SCATTERING, self.SCATTERING)
    
    angular_velocity = self.randomDirection() * self.TUMBLING
   
    self.physics.SetTransform(Matrix(position))
    self.physics.SetVelocity(velocity)
    self.physics.SetAngVelocity(angular_velocity)  
    return True  
  
class AimedMeteor(Meteor):
  #in seconds
  AIR_TIME = 1.
  
  # between 0 and 1 
  SCATTERING = 0.3
  
  MIN_SPEED = 50
  MAX_SPEED = 80
  MIN_SIZE = 0.5
  MAX_SIZE = 2.

  # between 0 and the radius of the meteor
  TUMBLING = 0.3
  
  def __init__(self,scene,aim,direction):
    u = random.uniform
    Meteor.__init__(self,scene,u(self.MIN_SIZE, self.MAX_SIZE))
    
    direction.x *= 1.+u(-self.SCATTERING, self.SCATTERING)
    direction.y *= 1.+u(-self.SCATTERING, self.SCATTERING)
    direction.z *= 1.+u(-self.SCATTERING, self.SCATTERING)

    velocity = direction * u(self.MIN_SPEED, self.MAX_SPEED)
    
    position = aim - velocity * self.AIR_TIME

    angular_velocity = self.randomDirection() * self.TUMBLING    
        
    self.physics.SetTransform(Matrix(position))
    self.physics.SetVelocity(velocity)
    self.physics.SetAngVelocity(angular_velocity)    
  
  
  def reset(self):
    return False


class TargetedMeteor(AimedMeteor):  
  def __init__(self,scene,target):
    aim = eRacer.ExtractPosition(target.transform)+target.velocity*1.2
    down = mul0(target.transform, Vector3(0,-1,0))
    AimedMeteor.__init__(self,scene,aim,down)
  
  
  def reset(self):
    return False