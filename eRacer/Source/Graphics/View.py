from Core.Globals import *
from Logic.Camera import Camera, OrthographicCamera
from Logic.HudQuad  import HudQuad

class View(object):
  def __init__(self, camera=None, renderables=None, viewport=None):
    self.viewport     = viewport    or (0, 0, game().graphics.width, game().graphics.height)
    self.camera       = camera      or Camera()
    self.camera.SetAspectRatio(self.viewport[2]/float(self.viewport[3]))
    self.renderables  = renderables or []
      
  def AddRenderable(self, obj):
    self.renderables.append(obj)
    return obj
    
  def Add(self, obj):
    return self.AddRenderable(obj)
    
  def Draw(self):
    gfx = game().graphics.graphics
    gfx.SetCamera(self.camera)
    gfx.SetViewport(*self.viewport)
    d3d = gfx.GetDevice()
    
    for i in self.renderables:
      i.Draw(d3d)
      
  def Clear(self):
    pass
          
class HudView(View):
  def __init__(self, renderables=None, viewport=None):
    self.viewport     = viewport    or (0, 0, game().graphics.width, game().graphics.height)
    self.camera       = OrthographicCamera(800,600)
    self.renderables  = renderables or []
    self.stringSprite = cpp.StringSprite()
    self.name = 'unnamed'
      
  def AddRenderable(self, obj):
    if isinstance(obj, HudQuad):
      obj.aspectRatio = float(self.viewport[3])/self.viewport[2]
      obj.Update()
      obj = obj.graphics
    return View.AddRenderable(self, obj)    
  
  def WriteString(self, text, family, size, viewportX, viewportY, color=cpp.WHITE):
    screenX = int(viewportX*(self.viewport[2]/800.0) + self.viewport[0])
    screenY = int(viewportY*(self.viewport[3]/600.0) + self.viewport[1])
    self.stringSprite.Write(text, family, int(size * (self.viewport[3]/600.0)), screenX, screenY, color)

  def Draw(self):
    View.Draw(self)
    d3d = game().graphics.graphics.GetDevice()
    
    self.stringSprite.Draw(d3d)  
    self.stringSprite.Clear()
    
  def Clear(self):
    self.stringSprite.Clear()
    
