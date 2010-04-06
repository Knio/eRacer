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
    w = self.stretch and self.width or self.width * self.aspectRatio
    self.transform = Matrix(
      Point3(self.left+self.width*.5, 600-self.top-self.height*.5, 0), 
      w, self.height, 1)
    self.graphics.SetTransform(self.transform)
    
  def SetCenter(self, x,y):
    self.left = x - width/2.0
    self.top  = y + height/2.0
    self.Update()

  def SetLeftTop(self, left, top):
    self.left = left
    self.top  = top
    self.Update()

  def SetSize(self, width, height):
    self.width = width
    self.height = height
    self.Update()
