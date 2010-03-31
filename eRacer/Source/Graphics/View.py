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
          
class HudView(View):
  def __init__(self, renderables=None, viewport=None):
    self.viewport     = viewport    or (0, 0, game().graphics.width, game().graphics.height)
    self.camera       = OrthographicCamera(800,600)
    self.renderables  = renderables or []
    self.stringSprite = cpp.StringSprite(*self.viewport)
      
  def AddRenderable(self, obj):
    if isinstance(obj, HudQuad):
      obj.aspectRatio = float(self.viewport[3])/self.viewport[2]
      obj.Update()
      print obj, obj.aspectRatio
      obj = obj.graphics
    return View.AddRenderable(self, obj)    
  
  def WriteString(self, text, family, size, pos, color=cpp.WHITE):
    self.stringSprite.Write(text, family, size, pos, color)

  def Draw(self):
    View.Draw(self)
    d3d = game().graphics.graphics.GetDevice()
    
    self.stringSprite.Draw(d3d)  
    self.stringSprite.Clear()
