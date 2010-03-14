from Core.Globals import *

import Track1
import Track2

class Track(Entity, cpp.Track):
  def __init__(self, name):
    Entity.__init__(self)
    cpp.Track.__init__(self)
    
    self.graphics = MeshNode("track")
    self.physics  = cpp.TriMesh()

    
    # HACK
    track = None
    # track = __import__(name)
    if name == 'Track1':
      track = Track1
    if name == 'Track2':
      track = Track2
    # END HACK
    
    for i in track.TRACK:
      self.Add(i)
    
    self.Subdivide(6)
    
    self.dist = self.GetTotalDist()
    
    profile = cpp.VectorTrackVertex()
    for i in track.PROFILE:
      profile.push_back(i)
    
    mesh = self.CreateMesh(profile)
    tex  = game().io.LoadTexture('ConcretePlates.jpg')
    mat  = game().graphics.graphics.DefaultMaterial()
    mesh.disown()
    mat.disown()
    self.mesh = cpp.Mesh(mesh, mat, tex)
    
    self.graphics.Init(self.mesh)
    self.physics.Init(mesh)
    self.physics.SetGroup(cpp.TRACK)
    
