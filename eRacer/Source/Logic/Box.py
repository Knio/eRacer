from Core.Globals import *

class Box(Prop):
  def __init__(self, position):
    Prop.__init__(self, 
        MeshNode('Box'), 
        'leather-box.x', 
        cpp.Box(False, 4000),
        tx=Matrix(position)
    )