from Game import Module
import eRacer

class Input(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.keyboard = eRacer.Keyboard()
    self.keyboard.Init(game.graphics.hwnd, game.graphics.hinst)
    
  def __getitem__(self, key):
    return self.keyboard.isKeyPressed(key)
    
  def isKeyPressed(self, key):
    return self.keyboard.isKeyPressed(key)

  def __getitem__(self, key):
    return self.keyboard.isKeyPressed(key)
    
  def Tick(self, time):
    Module.Tick(self, time)
    self.keyboard.Update()