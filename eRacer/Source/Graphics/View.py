from Core.Globals import *
from Logic.Camera import Camera

class View(object):
  def __init__(self, camera=None, renderables=None):
    self.camera       = camera      or Camera()
    self.renderables  = renderables or []
    
  def AddRenderable(self, obj):
    self.renderables.append(obj)
    return obj

  def Add(self, obj):
    return self.AddRenderable(obj)
    
  def Draw(self):
    gfx = dev = game().graphics.graphics
    gfx.SetCamera(self.camera.camera)
    d3d = gfx.GetDevice()
    
    for i in self.renderables:
      i.Draw(d3d)
    
