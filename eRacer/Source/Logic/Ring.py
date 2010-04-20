from Core.Globals import *

class Ring(Model):
  def __init__(self, name, gFilename, pFilename, tx=IDENTITY, *args):
    Model.__init__(self, name, gFilename, None, tx, *args)

    #physics model is disabled currently because it slows the game down too much
    def physLoad(pMesh):
      if pMesh:
        #self.physics = cpp.TriMesh()
        #self.physics.Init(pMesh)
        pass
    #game().io.LoadMeshAsync(physLoad, pFilename, *args)