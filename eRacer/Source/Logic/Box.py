from Game import Entity

import eRacer

class Box(Entity):
  def __init__(self, game):
     Entity.__init__(self, game)
     
     
     
     #self.physics = PhysicsObject()
     self.graphics = game.graphics.scene.CreateMovingGeometry("test")
     
     game.graphics.graphics.LoadGeometryTest(self.graphics, "Resources/Ship_06.x", "Resources/Ship_06.png")
     
     
     
  def Tick(self, time):
     pass