from Game.Module import Module
import eRacer
import os

PATH = 'Resources/Sounds'

class Sound(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.sound = eRacer.Sound()
    self.sound.Init()

  def PlaySound2D(self, file):
    self.sound.PlaySound2D(os.path.join(PATH,file))

  def Tick(self, time):
    Module.Tick(self, time)
    self.sound.Update()
    
  def Quit(self):
    Module.Quit(self)
    self.sound.StopMusic()
    self.sound.Shutdown()
