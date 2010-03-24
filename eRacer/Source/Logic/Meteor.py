import random
import math

from Core.Globals import *

class MeteorManager(object):
  VANISHING_DISTANCE = 600.
  N_CHANNELS= 5 

  def __init__(self, state):
    self.state = state
    self.meteors = []
    
    game().event.Register(self.MeteorMeteorCollisionEvent)
    game().event.Register(self.MeteorCarCollisionEvent)
    game().event.Register(self.MeteorTrackCollisionEvent)
    

    self.meteorfx = cpp.SoundFx();
    self.meteorfx.isLooping  = False
    self.meteorfx.is3D     = True
    self.meteorfx.isPaused = True
    self.meteorfx.minDist  = 70
    game().sound.sound.LoadSoundFx("MeteorCollision.wav", self.meteorfx)
    
    #self.collisionSounds = []
    #self.soundIndex = 0
    #for i in range(self.N_CHANNELS):
    #  sound = cpp.SoundFx();
    #  sound.isLooping  = False
    #  sound.is3D     = True
    #  sound.isPaused = True
    #  sound.volume   = 255
    #  sound.minDist  = 50
    #  game().sound.sound.LoadSoundFx("MeteorCollision.wav", sound)
    #  self.collisionSounds.append(sound)
    
    
  def spawnRandom(self):
    m = self.state.Add(RandomMeteor())
    self.meteors.append(m)
    return m
            
  def spawnTargeted(self, target):
    m = self.state.Add(TargetedMeteor(target))
    self.meteors.append(m)
    return m


  def spawnAimed(self, aim):
    m = self.state.Add(AimedMeteor(aim))
    self.meteors.append(m)
    return m

    
  def Tick(self, time):
    for meteor in self.meteors:
      pos = cpp.ExtractPosition(meteor.transform)
      if(length(pos)>self.VANISHING_DISTANCE):
        #print "respawning at position ",pos," with distance ",length(pos)," from origin." 
        if not meteor.reset():
          self.meteors.remove(meteor)
          self.state.Remove(meteor)
  
        
  def MeteorMeteorCollisionEvent(self, meteorId1, meteorId2, force):
    pass
    #print "MM Collision reported to MeteorManager"

  def MeteorTrackCollisionEvent(self, meteorId, trackId, force):
    meteor = self.state.entities[meteorId]
    meteor.hitTrack(force)
    self.meteorfx.position = mul1(meteor.transform, ORIGIN)
    self.meteorfx.velocity = ORIGIN #vel
    
    if length(force) > 1000000:
      game().sound.sound.PlaySoundFx(self.meteorfx)
    #print "MC Collision reported to MeteorManager"
    
    #sound = self.collisionSounds[self.soundIndex]
    #sound.position = mul1(meteor.transform, ORIGIN)
    #sound.velocity = ORIGIN #vel
    #sound.isPaused = False
    #game().sound.sound.UpdateSoundFx(sound)
    #self.soundIndex = (self.soundIndex+1)%self.N_CHANNELS
    
  def MeteorCarCollisionEvent(self, meteorId, carId, force):
    pass
    print "MC Collision reported to MeteorManager"

  def Release(self):
    self.meteors = []
    self.state = None
    game().event.UnRegister(self.MeteorMeteorCollisionEvent)
    game().event.UnRegister(self.MeteorCarCollisionEvent)
    game().event.UnRegister(self.MeteorTrackCollisionEvent)

class Meteor(Model):
  DENSITY   = 200.
  MODELS    = ["Meteor1.x", "Meteor2.x", "Meteor3.x"]

  def __init__(self, scale=1):    
    Model.__init__(
      self,
      'Meteor',
      random.choice(self.MODELS),
      cpp.Box(True, self.DENSITY*scale*scale, ORIGIN, IDENTITY, Vector3(scale,scale,scale)),      
    )

    #hack: scale has to be stored separately because physiscs will keep overriding it 
    self.scale = scale
    self.physics.SetGroup(cpp.METEOR)
    self.physics.SetId(self.id)

        
  def reset(self):
    return True
    
  def hitTrack(self,force):
    if(self.scale > 1.0 and self.scale<3.5):
      self.physics.PutToSleep()
      direction = normalized(force)
      pos = self.physics.GetPosition()-direction*self.scale
      #self.physics.SetVelocity(Vector3(0,0,0))
      #self.physics.SetAngVelocity(Vector3(0,0,0));
      self.physics.SetPosition(pos)
        
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.transform = self.physics.GetTransform()    
    self.graphics.SetTransform(cpp.Scaled(self.transform, self.scale,self.scale,self.scale))  

  def randomDirection(self):
    u = random.uniform
    return normalized(Vector3(u(-1.,1.),u(-1.,1.),u(-1.,1.)))
    
  def scatter(self, point, scattering):
    u = random.uniform
    point.x *= 1.+u(-scattering, scattering)
    point.y *= 1.+u(-scattering, scattering)
    point.z *= 1.+u(-scattering, scattering)    

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
  
  def __init__(self):
    Meteor.__init__(self, random.uniform(self.MIN_SIZE,self.MAX_SIZE))
    
    self.reset()
    
    
  def reset(self):
    u = random.uniform
    
    # the meteor is spawned in a random location on a sphere
    direction = self.randomDirection()
    position = direction * self.SPAWNING_DISTANCE
    
    velocity = direction * -(u(self.MIN_SPEED, self.MAX_SPEED)/math.sqrt(self.scale))

    self.scatter(velocity, self.SCATTERING)
    
    angular_velocity = self.randomDirection() * self.TUMBLING
   
    self.physics.SetTransform(Matrix(position))
    self.physics.SetVelocity(velocity)
    self.physics.SetAngVelocity(angular_velocity)  
    return True  
  
class AimedMeteor(Meteor):
  #in seconds
  AIR_TIME = 1.
  
  # between 0 and 1 
  DIR_SCATTERING = 0.5
  POS_SCATTERING = 0.2
  
  MIN_SPEED = 50
  MAX_SPEED = 80
  MIN_SIZE = 0.5
  MAX_SIZE = 2.

  # between 0 and the radius of the meteor
  TUMBLING = 0.3
  
  def __init__(self, aim, direction):
    u = random.uniform
    Meteor.__init__(self, u(self.MIN_SIZE, self.MAX_SIZE))
    
    self.scatter(direction,self.DIR_SCATTERING)
    
    velocity = direction * u(self.MIN_SPEED, self.MAX_SPEED)
    
    position = aim - velocity * self.AIR_TIME

    self.scatter(position, self.POS_SCATTERING)

    angular_velocity = self.randomDirection() * self.TUMBLING    
        
    self.physics.SetTransform(Matrix(position))
    self.physics.SetVelocity(velocity)
    self.physics.SetAngVelocity(angular_velocity)    
    
  def reset(self):
    return False


class TargetedMeteor(AimedMeteor):  
  def __init__(self, target):
    aim = cpp.ExtractPosition(target.transform)+target.velocity*1.2
    down = mul0(target.transform, Vector3(0,-1,0))
    AimedMeteor.__init__(self, aim, down)
  
  
  def reset(self):
    return False
