from Game import Module
import eRacer

class Sound(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.sound = eRacer.Sound()
    self.sound.Init()

  def PlaySound2D(self, *args):
    self.sound.PlaySound2D(*args)

  def Tick(self, time):
    Module.Tick(self, time)
    self.sound.Update()
    
  def Quit(self):
    Module.Quit(self)
    self.sound.StopMusic()
    self.sound.Shutdown()
