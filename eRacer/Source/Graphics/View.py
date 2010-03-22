from Core.Globals import *
from Logic.Camera import Camera

class View(object):
  def __init__(self, camera=None, renderables=None, viewport=None):
    self.viewport     = viewport    or (0, 0, game().graphics.width, game().graphics.height)
    self.camera       = camera      or Camera()
    self.renderables  = renderables or []
    self.stringSprite = cpp.StringSprite(*self.viewport)
    self.renderables.append(self.stringSprite)
      
  def AddRenderable(self, obj):
    self.renderables.append(obj)
    return obj
    
  def WriteString(self, text, family, size, pos, color=cpp.WHITE):
    self.stringSprite.Write(text, family, size, pos, color)

  def Add(self, obj):
    return self.AddRenderable(obj)
    
  def Draw(self):
    gfx = dev = game().graphics.graphics
    gfx.SetCamera(self.camera)
    gfx.SetViewport(*self.viewport)
    d3d = gfx.GetDevice()
    
    for i in self.renderables:
      i.Draw(d3d)
      
    self.stringSprite.Clear()
