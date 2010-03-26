from Core.Globals import *

class Camera(Entity, cpp.Camera):
  def __init__(self):
    Entity.__init__(self)
    cpp.Camera.__init__(self)
    
    self.SetNear(2)
    #very high for now, can be reduced once we have some fogging
    self.SetFar(4000)
    self.SetFovY(math.pi/4.0)
    self.SetAspectRatio(8./6.)

  def Tick(self, time):
    Entity.Tick(self, time)

class OrthographicCamera(Camera):
  def __init__(self, width, height):
    Entity.__init__(self)
    cpp.Camera.__init__(self)
    position = Point3(width/2., height/2., -2.)
    lookAt   = Point3(width/2., height/2.,  0.)
    self.SetFrame(position, lookAt, Y, False)
    self.SetHeight(height)
    self.SetAspectRatio(float(width)/height)
    self.SetNear(1)
    self.SetFar(3)
    
  def Tick(self, time):
    Camera.Tick(self, time)

class CirclingCamera(Camera):
  def __init__(self):
    Camera.__init__(self)
    
  def Tick(self, time):
    Camera.Tick(self, time)
    
    # spin around a circle, looking at the origin
    t = time.seconds / 10
    pos = Vector3(
      math.cos(t), 
      math.sin(t)*0.5+1, 
      math.sin(t)
    )
    pos *= 20
    
    self.SetPosition(pos)

class ChasingCamera(Camera):
  MAX_FOV = PI*7./8.  
    
  def __init__(self, target): 
    Camera.__init__(self)
    self.target   = target
    self.position = Point3(0, 50, -1)
    self.fov      = math.pi/2.1
    self.upworld  = Y
    # self.SetFar(40)
    
    
  
  def Tick(self, time):
    Camera.Tick(self, time)
    
    vel = length(self.target.physics.GetLocalPointWorldVelocity(ORIGIN))
  

    capBack = math.pow(vel, 0.2)
    originlocal = ORIGIN
    behindlocal = Point3(0,6,-12-0);
    originworld = mul1(self.target.transform, originlocal)
    behindworld = mul1(self.target.transform, behindlocal)
    forwardlocal = Point3(0,0,15);
    forwardworld = mul1(self.target.transform, forwardlocal)
    
    fov = max(math.pi/2.5/(vel*0.01+1), math.pi/3)
    
    alpha = math.pow(0.01, float(time.game_delta) / time.RESOLUTION)
    if self.target.crashtime > 0:
      alpha = 0.98;
    self.position = self.position*alpha + behindworld*(1-alpha)
    self.fov      = self.fov*alpha      + fov*(1-alpha)
    
    self.fov = min(self.fov, self.MAX_FOV/self.GetAspectRatio())

    alpha = math.pow(0.6, float(time.game_delta) / time.RESOLUTION)
    if self.target.crashtime > 0:
      alpha = 0.99;
    upworld = mul0(self.target.transform, Y*10)
    self.upworld = self.upworld*alpha + upworld*(1-alpha)
    
    # lateral = project(self.position-originworld, mul1(self.target.transform, X))
    # pos = self.position - lateral * 2
    
    pos = self.position
    
    
    #self.SetPosition(pos)    
    self.SetFovY(self.fov)
    
    targetPosition  = cpp.ExtractPosition(self.target.transform)
    #targetPosition.y += 5
    
    #self.SetLookAt(targetPosition)
    self.SetFrame(pos, forwardworld, self.upworld)
    
class CarCamera(Camera):
  def __init__(self, target): 
    Camera.__init__(self)
    self.target   = target
    self.position = Point3(0, 0, 0)
    self.fov      = math.pi/2.1
    
  def Tick(self, time):
    Camera.Tick(self, time)
    
    posworld  = mul1(self.target.transform, Point3(0,1,1))
    lookworld = mul1(self.target.transform, Point3(0,1,2))
    upworld   = mul0(self.target.transform, Y)
    
    self.SetFrame(posworld, lookworld, upworld)
      

class FirstPersonCamera(Camera):
  def __init__(self): 
    Camera.__init__(self)

    self.position = Point3(0,0,0)
    self.velocity = Vector3(0,0,0)
    self.look   = 0.
    
    self.transform = Matrix()
    
    game().event.Register(self.CameraAccelerateEvent)
    game().event.Register(self.CameraStrafeEvent)
    game().event.Register(self.CameraLookAroundEvent)
    
  def CameraAccelerateEvent(self, acceleration):  
    self.velocity = self.velocity + Z * acceleration
    
  def CameraStrafeEvent(self, acceleration):
    self.velocity = self.velocity + X * acceleration
    
  def CameraLookAroundEvent(self, relX, relY):
    x = self.look.real + relX
    y = self.look.imag + relY
    y = clamp(y, -PI/2+ZERO, PI/2-ZERO)
    self.look = complex(x,y)
    
    self.transform  = Matrix(ORIGIN, self.look.imag, X) \
                    * Matrix(ORIGIN, self.look.real, Y)
    
  def Tick(self, time):
    Camera.Tick(self, time)
    delta = float(time.wall_delta) / time.RESOLUTION
        
    velocity        = mul0(self.transform, self.velocity)
    self.position   = self.position + velocity * (delta * 100.0)
    lookat          = self.position + mul0(self.transform, Z)
    
    # print self.look
    # print self.position
    # printvec(lookat)
    
    self.SetFrame(self.position, lookat)
    
