from Game     import Module
from Window   import Window
import eRacer

class Graphics(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    
    self.window   = Window("Test")
    self.graphics = eRacer.GraphicsLayer.GetGraphicsInstance()
    
    self.hwnd = self.window.hwnd
    self.hwnd.disown()
    self.hinst = self.window.hinst
    self.hinst.disown()
    
    self.scene = eRacer.Scene()
    
    self.graphics.Init(self.hwnd)

  def Tick(self, time):
    Module.Tick(self, time)
    self.window.Poll()
    self.graphics.RenderFrame()
  
  def Quit(self):
    Module.Quit(self)
    # TODO close window
    