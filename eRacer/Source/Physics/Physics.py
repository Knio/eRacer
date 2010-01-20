from Game import Module
import eRacer

class Physics(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.physics = eRacer.PhysicsLayer()
    self.physics.InitSDK()
    
  def Tick(self, time):
    Module.Tick(self, time)
    
  def Quit(self):
    Module.Quit(self)
    self.physics.ReleaseSDK()
