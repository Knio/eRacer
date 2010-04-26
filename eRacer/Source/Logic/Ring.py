from Core.Globals import *

class Ring(StaticModel):
  def __init__(self, name, gFilename, pFilename, tx=IDENTITY, *args):
    StaticModel.__init__(self, name, gFilename, tx, *args)
    self.graphics.setTint(Vector4(1.0,1.0,1.0,1.0))
    #physics model is disabled currently because it slows the game down too much
    def physLoad(pMesh):
      if pMesh:
        #self.physics = cpp.TriMesh()
        #self.physics.Init(pMesh)
        pass
    #game().io.LoadMeshAsync(physLoad, pFilename, *args)
