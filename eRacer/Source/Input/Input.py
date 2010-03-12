from Core.Globals import *

class Input(cpp.Input):
  def __init__(self, game):
    cpp.Input.__init__(self)
    self.input.Init(game.graphics.hwnd, game.graphics.hinst) 
    
  def Tick(self, time):
    Module.Tick(self, time)
    self.input.Update()