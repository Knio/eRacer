from Game     import Module
from Window   import Window
import eRacer

class Graphics(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    
    self.window   = Window("Test")
    self.graphics = eRacer.GraphicsLayer.GetGraphicsInstance()
    
    # get pointers
    
    self.hwnd = self.window.hwnd
    self.hwnd.disown()
    self.hinst = self.window.hinst
    self.hinst.disown()
    
    self.graphics.Init(self.hwnd)
    
    self.d3d = self.graphics.GetDevice()
    self.d3d.disown()
    
    self.scene = eRacer.Scene()
    self.graphics.m_scene = self.scene
    self.camera = None

  def SetCamera(self, camera):
    self.camera = camera

  def Tick(self, time):
    Module.Tick(self, time)
    self.window.Poll()
    if self.camera:
        self.graphics.SetCamera(self.camera.camera)
    else:
        self.graphics.SetCamera()
    self.graphics.RenderFrame()
  
  def Quit(self):
    Module.Quit(self)
    # TODO close window
    