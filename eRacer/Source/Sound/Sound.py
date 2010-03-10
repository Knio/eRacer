from Game.Module import Module
import eRacer as cpp
import os

PATH = 'Resources/Sounds'

class Sound(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.sound = cpp.Sound()
    self.sound.Init()

  def PlaySound2D(self, file):
    self.sound.PlaySound2D(os.path.join(PATH,file))

  def Tick(self, time):
    Module.Tick(self, time)
    self.sound.Update()
    
    
  def Quit(self):
    Module.Quit(self)
    #this method does not exist anymore. Don or Tom should check whether it needs replacement
    #self.sound.StopMusic()
    self.sound.Shutdown()
