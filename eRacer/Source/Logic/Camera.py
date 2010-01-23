from Game import Entity
import eRacer
import math


class Camera(Entity):
  def __init__(self, game):
  	Entity.__init__(self, game)
  	self.camera = eRacer.Camera()
  	
  	self.camera.SetFar(100)
  	self.camera.SetFovY(math.pi/4.0)
  	self.camera.SetAspectRatio(8./6.)
  	
  	
  def Tick(self, time):
    Entity.Tick(self, time)
    
    # spin around a circle of radius 10, looking at the origin
    pos = eRacer.D3DXVECTOR3(
      math.cos(time.seconds), 
      0.5, 
      math.sin(time.seconds)
    )
    pos *= 10
    
    view = self.camera.GetViewMatrix()
    proj = self.camera.GetProjectionMatrix()
    
    self.camera.SetPosition(pos)
    