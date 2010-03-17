from Core.Globals import *

class Box(Model):
  def __init__(self, position):
    Model.__init__(self, 
        'Box', 
        'LeatherBox.x', 
        cpp.Box(True, 4000),
        tx=Matrix(position)
    )