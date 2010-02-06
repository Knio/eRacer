from Core.Globals import *

class Starfield(Entity, eRacer.Starfield):
  def __init__(self, scene, camera, n, size):
    Entity.__init__(self)
    eRacer.Starfield.__init__(self, n, size)
    self.camera = camera
    scene.AddRenderable(self)
    
  def Tick(self, time):
    Entity.Tick(self, time)
    v = self.camera.camera.GetViewMatrix()
    p = self.camera.camera.GetPosition()
    print p.x, p.y, p.z
    self.Update(v, p)
    
