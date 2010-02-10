from Core.Globals import *

class SkyBox(eRacer.SkyBox):
  def __init__(self, view):
    self.view = view
    eRacer.SkyBox.__init__(self, view.camera)
    self.camera = view.camera
    view.AddRenderable(self)
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.initialized = True
      
    game().io.LoadMeshAsync(load, self, "skybox2.x")   
    
