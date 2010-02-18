from Core.Globals import *

class Track(Entity):
  def __init__(self, scene):
    Entity.__init__(self)

    self.physics = eRacer.TriMesh()
    self.graphics = scene.CreateStaticMeshNode("track")
    # self.graphics.visible = False
        
    def load(r):
      if r:
        print 'Failed to load mesh!!'
        return
      self.graphics.initialized = True
      print "num faces in track: ",self.graphics.GetMesh()
      self.physics.Init(self.graphics.mesh());
      
    # game().io.LoadMeshAsync(load, self.graphics, "Track.x")   
    game().io.LoadMeshAsync(load, self.graphics, "Track1.x")
    
    
  def Tick(self, time):
     Entity.Tick(self, time)
