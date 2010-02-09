from Core.Globals import *

class SkyBox(eRacer.SkyBox):
  def __init__(self, camera):
    eRacer.SkyBox.__init__(self, camera)
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.initialized = True
      
    game().io.LoadMeshAsync(load, self, "skybox2.x")   
    
