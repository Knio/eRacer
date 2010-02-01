from Game import Module
import eRacer

class Input(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.input = eRacer.Input()
    self.input.Init(game.graphics.hwnd, game.graphics.hinst)
    
  def __getitem__(self, key):
    return self.input.isKeyPressed(key)
    
  def isKeyPressed(self, key):
    return self.input.isKeyPressed(key)

  def __getitem__(self, key):
    return self.input.isKeyPressed(key)
    
  def Tick(self, time):
    Module.Tick(self, time)
    self.input.Update()