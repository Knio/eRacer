from Core.Globals import *

class SkyBox(eRacer.SkyBox):
  def __init__(self):
    # why can python not call default constructor?
    eRacer.SkyBox.__init__(self)
    
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.initialized = True      
    game().io.LoadMeshAsync(load, self, "skybox2.x")   
    
