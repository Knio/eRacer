from Core.Globals import *

class Quad(cpp.QuadNode,Entity):
  def __init__(self, view, texture):
    Entity.__init__(self)
    cpp.QuadNode.__init__(self, "Quad")
    self.transform = Matrix()
       
    t = game().io.LoadTexture(texture)   
    self.Init(t)
    view.AddRenderable(self)
            
    
  def Tick(self, time):
    self.SetTransform(self.transform)