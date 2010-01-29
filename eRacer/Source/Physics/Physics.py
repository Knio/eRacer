from Game import Module
import eRacer

class Physics(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.physics = eRacer.PhysicsLayer()
    self.physics.InitSDK()
    self.physics.InitScene()
    self.physics.FinalizeSDK()
    
  def Start(self):
    Module.Start(self)
    
  def Tick(self, time):
    Module.Tick(self, time)
    self.physics.GetPhysicsResults()
    self.physics.UpdatePhysics(time)
    
  def Quit(self):
    Module.Quit(self)
    self.physics.ReleaseSDK()
