from Core.Globals import *

class Sprite(eRacer.Sprite,Entity):
  def __init__(self, view):
    Entity.__init__(self)
    eRacer.Sprite.__init__(self)
    
        
    t = game().io.LoadTexture("eracerx_logo_negative.png")   
    self.Init(t)
    view.AddRenderable(self)
            
  def transform_changed(self):
    self.SetTransform(self.transform)