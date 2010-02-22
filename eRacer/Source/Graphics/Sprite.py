from Core.Globals import *

class Sprite(eRacer.Sprite,Entity):
  def __init__(self, view,position):
    Entity.__init__(self)
       
    self.set_translation(position)   
    #self.SetTransform(self.transform)
        
    t = game().io.LoadTexture("eracer_logo.png")   
    self.Init(t)    
    view.AddRenderable(self)
            
  def transform_changed(self):
  	pass
    # self.SetTransform(self.transform)