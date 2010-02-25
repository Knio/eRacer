from Core.Globals import *

class Sprite(eRacer.Sprite,Entity):
  def __init__(self, view):
    Entity.__init__(self)
    eRacer.Sprite.__init__(self)
    self.transform = Matrix()
  
        
    t = game().io.LoadTexture("eracerx_logo_negative.png")   
    self.Init(t)
    view.AddRenderable(self)
            
  def set_translation(self, translation):
    eRacer.SetTranslation(self.transform, translation)
    self.transform_changed()
            
            
  def scale(self, x,y,z):
    self.transform = eRacer.Scaled(self.transform,x,y,z)
    self.transform_changed()   

  def transform_changed(self):
    self.SetTransform(self.transform)