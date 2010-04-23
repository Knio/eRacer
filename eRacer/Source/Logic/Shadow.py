from Core.Globals import *
from Quad import Quad

class Shadow(Quad):
  SCALING = 12
  def __init__(self, target):
    Quad.__init__(self, 'Shadow', 'Shadow2.png', Matrix(Point3(0,0,0),90,X,self.SCALING,self.SCALING,self.SCALING))
    self.target = target
    
  def Tick(self, time):
    f = self.target.frame
    p = mul1(self.target.transform, ORIGIN)
    pos = projectOnto(p - f.position, f.up) + f.position + f.up*0.3
    self.transform = Matrix(pos, f.fw, -f.up,self.SCALING)
    Quad.Tick(self, time)
    