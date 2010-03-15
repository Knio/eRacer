from Core.Globals import *
from Graphics.Quad import Quad

class Shadow(Quad):
  def __init__(self, target):
    Quad.__init__(self, 'Shadow', 'Shadow.png', Matrix(Point3(0,0,0),90,X,11,11,11))
    self.target = target
    
  def Tick(self, time):
    f = self.target.frame
    p = mul1(self.target.transform, ORIGIN)
    pos = projectOnto(p - f.position, f.up) + f.position + f.up*0.3
    self.transform = Matrix(pos, f.fw, -f.up,11)
    Quad.Tick(self, time)
    