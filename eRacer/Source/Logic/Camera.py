from Game import Entity
import eRacer
import math


class Camera(Entity):
  def __init__(self, game):
  	Entity.__init__(self, game)
  	self.camera = eRacer.Camera()
  	
  	self.camera.SetFar(100)
  	self.camera.SetFovY(math.pi/3.0)
  	self.camera.SetAspectRatio(8./6.)
  	
  	
  def Tick(self, time):
  	Entity.Tick(self, time)
  	pos = eRacer.D3DXVECTOR3(math.cos(time.seconds), 0.5, math.sin(time.seconds))
	pos *= 10
  	eye = eRacer.D3DXVECTOR3(0, 0, 0)
  	
  	view = self.camera.GetViewMatrix()
  	proj = self.camera.GetProjectionMatrix()

  	self.camera.SetPosition(pos)

  	
