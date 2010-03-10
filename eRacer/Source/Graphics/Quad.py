from Core.Globals import *

class Quad(cpp.Quad,Entity):
  def __init__(self, view, texture):
    Entity.__init__(self)
    cpp.Quad.__init__(self)
    self.transform = Matrix()
       
    t = game().io.LoadTexture(texture)   
    self.Init(t)
    view.AddRenderable(self)
            
    
  def Tick(self, time):
    self.SetTransform(self.transform)