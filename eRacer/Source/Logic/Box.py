from Game import Entity

class Box(Entity):
  def __init__(self, game):
     Entity.__init__(self, game)
     
     self.pos = Point3(0, 0, 10)
     self.rot = Vector3(0, 0, 0)
     
     self.physics = PhysicsObject()
     self.graphics = game.graphics.GetNode(self.pos, self.rot, moving=True)
     
     def loaded(mesh):
     self.graphics.SetMesh(mesh)
     
     self.game.io.LoadMesh(loaded, "box.obj")
   
     
  def Tick(self, time):
     self.pos = self.physics.GetPos()
     self.rot = self.physics.GetRot()
     
     self.graphics.SetPos(self.pos)
     self.graphics.SetRot(self.rot)
     