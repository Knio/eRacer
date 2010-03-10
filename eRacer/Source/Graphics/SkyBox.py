from Core.Globals import *

class SkyBox(cpp.SkyBox):
  def __init__(self):
    # why can python not call default constructor?
    cpp.SkyBox.__init__(self)
    
    def load(mesh):
      if mesh:
        self.Init(mesh)

    game().io.LoadMeshAsync(load, "skybox2.x")   
    
