from Core.Globals import *

class SkyBox(eRacer.SkyBox):
  def __init__(self, camera):
    eRacer.SkyBox.__init__(self, camera)
    self.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.visible = True
      
    game().io.LoadMeshAsync(load, self.graphics, "skybox2.x")   
    
