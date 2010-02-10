from Core.Globals import *

class Track(Entity):
  def __init__(self, scene):
    Entity.__init__(self)

    self.physics = eRacer.TriMesh()
    self.graphics = scene.CreateStaticGeometry("track")
    # self.graphics.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.initialized = True
      self.physics.Init(self.graphics.mesh());
      
    game().io.LoadMeshAsync(load, self.graphics, "track_fix.x")   
    
    
  def Tick(self, time):
     Entity.Tick(self, time)
