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
    p = Point3(0,0,0)
    p.x = pos.x  * (self.viewport[2]/800.0)
    p.y = pos.y  * (self.viewport[3]/600.0)
    p = p + Vector3(self.viewport[0], self.viewport[1], 0)
    self.stringSprite.Write(text, family, int(size * (self.viewport[3]/600.0)), p, color)

  def Draw(self):
    View.Draw(self)
    d3d = game().graphics.graphics.GetDevice()
    
    self.stringSprite.Draw(d3d)  
    self.stringSprite.Clear()
