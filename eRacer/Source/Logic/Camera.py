from Core.Globals import *

class Camera(Entity):
  def __init__(self, game):
  	Entity.__init__(self, game)
  	self.camera = eRacer.Camera()
  	
  	self.camera.SetFar(100)
  	self.camera.SetFovY(math.pi/4.0)
  	self.camera.SetAspectRatio(8./6.)
  	
  	
  def Tick(self, time):
    Entity.Tick(self, time)

class CircelingCamera(Camera):
  def __init__(self, game):
    Camera.__init__(self, game)
    
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
  def __init__(self, game, target): 
       Camera.__init__(self, game)
       self.target = target
  
  def Tick(self, time):
    Camera.Tick(self, time)