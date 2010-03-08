from Core.Globals import *

class Quad(eRacer.Quad,Entity):
  def __init__(self, view, texture):
    Entity.__init__(self)
    eRacer.Quad.__init__(self)
    self.transform = Matrix()
  
        
    t = game().io.LoadTexture(texture)   
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