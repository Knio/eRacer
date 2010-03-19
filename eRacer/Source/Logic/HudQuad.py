from Core.Globals import *
from Quad         import Quad

class HudQuad(Quad):
  def __init__(self, name, texture, left, top, width, height):
    self.left = left
    self.top = top
    self.width = width
    self.height = height
    p = Point3(left+width/2., game().window.height-(top+height/2.), 0)
    t = Matrix(p, width, height, 1)
    
    Quad.__init__(self, name, texture,t)
    
  def SetCenter(self, x,y):
    cpp.SetTranslation(self.transform,Point3(x,game().window.height-y,0))
    
  def SetLeftTop(self, left,top):
    self.SetCenter(x+self.width/2., y+self.height/2.)
    
  def SetSize(self, width, height):
    p = Point3(self.left+width/2., game().window.height-(self.top+height/2.), 0)
    t = Matrix(p, width, height, 1) 
    self.graphics.SetTransform(t)   
    
      
    
  
  