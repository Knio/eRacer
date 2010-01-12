from Game     import Module
from Window   import Window
import eRacer

class Graphics(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    
    self.window   = Window("Test")
    self.graphics = eRacer.GraphicsLayer.GetGraphicsInstance()
    
    h = self.window.hwnd
    h.disown()
    self.graphics.Init(h)

  def tick(self, time):
    if not self.window.Poll():
      self.game.state = 0
    self.graphics.RenderFrame()
  
