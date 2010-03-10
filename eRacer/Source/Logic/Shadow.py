from Core.Globals import *

class Shadow(Prop):
  def __init__(self, target):
    Prop.__init__(self, MovingMeshNode('Shadow'), 'shadow.x')
    self.target = target
    
  def Tick(self, time):
    f = self.target.frame
    p = mul1(self.target.transform, ORIGIN)
    pos = projectOnto(p - f.position, f.up) + f.position + f.up*0.3
    self.transform = Matrix(pos, f.up, f.fw)
    Prop.tick(self, time)
    