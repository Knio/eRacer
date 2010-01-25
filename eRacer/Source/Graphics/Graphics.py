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

  def Init(self):
    Module.Init(self)
    self.scene.LoadSkyBox('skybox.x')

  def SetCamera(self, camera):
    self.camera = camera

  def Tick(self, time):
    Module.Tick(self, time)
    self.window.Poll()
    self.graphics.RenderFrame(self.camera.camera, self.scene)
  
  def Quit(self):
    Module.Quit(self)
    # TODO close window
    