from Game import Module
import eRacer

class Input(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    #self.keyboard = eRacer.Keyboard()
    #print 1
    #self.keyboard.Init(game.graphics.hwnd, game.graphics.hinst)
    #print 2
    
  def Tick(self, time):
    Module.Tick(self, time)
    #self.keyboard.Update()