from Core.Globals import *

class Shadow(Model):
  def __init__(self, target):
    Model.__init__(self, 'Shadow', 'shadow.x')
    self.target = target
    
  def Tick(self, time):
    f = self.target.frame
    p = mul1(self.target.transform, ORIGIN)
    pos = projectOnto(p - f.position, f.up) + f.position + f.up*0.3
    self.transform = Matrix(pos, f.up, f.fw)
    Model.Tick(self, time)
    