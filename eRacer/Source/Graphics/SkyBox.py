from Core.Globals import *

class SkyBox(eRacer.SkyBox):
  def __init__(self, camera):
    eRacer.SkyBox.__init__(self, camera.camera)
    self.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.visible = True
      
    game().io.LoadMeshAsync(load, self, "skybox2.x")   
    
