from Core.Globals import *

class Camera(Entity):
  def __init__(self):
    Entity.__init__(self)
    self.camera = eRacer.Camera()
    
    self.camera.SetNear(2)
    self.camera.SetFar(2000) 
    self.camera.SetFovY(math.pi/4.0)
    self.camera.SetAspectRatio(8./6.)

  def Tick(self, time):
    Entity.Tick(self, time)


class CirclingCamera(Camera):
  def __init__(self):
    Camera.__init__(self)
    
  def Tick(self, time):
    Camera.Tick(self, time)
    
    # spin around a circle, looking at the origin
    t = time.seconds / 10
    pos = eRacer.D3DXVECTOR3(
      math.cos(t), 
      math.sin(t)*0.5+1, 
      math.sin(t)
    )
    pos *= 20
    
    self.camera.SetPosition(pos)


class ChasingCamera(Camera):
  def __init__(self, target): 
    Camera.__init__(self)
    self.target   = target
    self.position = Point3(0, 50, -1)
    self.fov      = math.pi/2.1
  
  def Tick(self, time):
    Camera.Tick(self, time)
    
    vel = length(self.target.physics.GetPointVelocity(ORIGIN))
    
    behind = Point3(0,4,-10-vel)
    behind = eRacer.mul1(self.target.transform, behind)
    
    fov = math.pi/2.5/(vel*0.10+1)
    
    alpha = math.pow(0.5, float(time.game_delta) / time.RESOLUTION)
    self.position = self.position*alpha + behind*(1-alpha)
    self.fov      = self.fov*alpha      + fov*(1-alpha)
    
    self.camera.SetPosition(self.position)
    self.camera.SetFovY(self.fov)
    targetPosition = Point3()
    eRacer.ExtractPosition(self.target.transform, targetPosition)
    
    self.camera.SetLookAt(targetPosition)
    
class FirstPersonCamera(Camera):
  def __init__(self): 
    Camera.__init__(self)

    self.set_translation(Vector3(0, 20, 20))
    self.acceleration = Vector3(0,0,0)
    self.speed = 200
    
    game().event.Register(self.CameraAccelerateEvent)
    game().event.Register(self.CameraStrafeEvent)
    game().event.Register(self.CameraLookAroundEvent)
    
  def CameraAccelerateEvent(self, acceleration):  
    self.acceleration.z += acceleration*self.speed
    
  def CameraStrafeEvent(self,acceleration):
    self.acceleration.x += acceleration*self.speed
    
    
  def CameraLookAroundEvent(self, relX, relY):
    self.rotate(-relX,-relY,0)
    
  def Tick(self, time):
    Camera.Tick(self, time)
    delta = float(time.wall_delta) / time.RESOLUTION
    
    acc = Vector3(self.acceleration)
    acc.x*=delta
    acc.z*=delta
    self.translate(acc)
    
  def transform_changed(self):
    # t = self.get_translation()
    # print t.x,",",t.y,",",t.z
    # eRacer.debug(self.transform)
    self.camera.SetViewMatrix(self.transform)  
