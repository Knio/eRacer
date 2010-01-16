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
    self.graphics.Init(self.hwnd)

  def Tick(self, time):
    Module.Tick(self, time)
    if not self.window.Poll():
      # post quit message
      self.game.state = 0
      return
    self.graphics.RenderFrame()
  
