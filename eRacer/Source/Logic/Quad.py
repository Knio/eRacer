from Core.Globals import *

class Quad(Entity):
  def __init__(self, name, texture, transform = IDENTITY):
    Entity.__init__(self)
    self.graphics = cpp.QuadNode(name, transform)
    self.transform = transform

    self.graphics.Init(game().io.LoadTexture(texture))
    
  def Tick(self, time):
    Entity.Tick(self, time)
    self.graphics.SetTransform(self.transform)