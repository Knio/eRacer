from Core.Globals import *

class Box(Model):
  def __init__(self, position):
    Model.__init__(self, 
        'Box', 
        'leather-box.x', 
        cpp.Box(False, 4000),
        tx=Matrix(position)
    )