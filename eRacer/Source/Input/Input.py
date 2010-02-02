from Core.Globals import *

class Input(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.input = eRacer.Input()
    self.input.Init(game.graphics.hwnd, game.graphics.hinst)
    
  def __getitem__(self, key):
    return self.input.isKeyDown(key)
    
  def isKeyDown(self, key):
    return self.input.isKeyDown(key)
    
  def Tick(self, time):
    Module.Tick(self, time)
    self.input.Update()