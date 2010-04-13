from Core.Globals import *
from Quad         import Quad

class HudQuad(Quad):
  def __init__(self, name, texture, left, top, width, height, stretch=False):
    self.left   = left
    self.top    = top
    self.width  = width
    self.height = height
    self.stretch = stretch
    self.aspectRatio = 1.0
    Quad.__init__(self, name, texture)
    self.Update()

  def Update(self):
    # if we don't want stretch, we have to account for the 
    # quotient of the actual aspect ratio and the assumed one (4:3).  
    w = self.stretch and self.width or self.width * self.aspectRatio * (4./3.)
    
    self.transform = Matrix(
      Point3(self.left+w*.5, 600-self.top-self.height*.5, 0), 
      w, self.height, 1)
    self.graphics.SetTransform(self.transform)
    
  def SetCenter(self, x,y):
    self.left = x - self.width/2.0
    self.top  = y - self.height/2.0
    self.Update()

  def SetLeftTop(self, left, top):
    self.left = left
    self.top  = top
    self.Update()

  def SetSize(self, width, height):
    self.width = width
    self.height = height
    self.Update()
