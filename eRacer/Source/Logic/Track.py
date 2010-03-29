from Core.Globals import *

import Track1
import Track2

class Track(Entity, cpp.Track):
  def __init__(self, name):
    Entity.__init__(self)
    cpp.Track.__init__(self)
    
    self.graphics = []
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
    
    meshes = self.CreateMesh(profile)
    
    for i in xrange(meshes.size()):
      tex   = game().io.LoadTexture('ConcretePlates.jpg')
      mat   = game().graphics.graphics.DefaultMaterial()
      mat.disown()
      
      g = MeshNode("track")
      g.Init(cpp.Mesh(mesh, mat, tex))
      self.graphics.append(g)
      
      p = cpp.TriMesh()
      p.Init(mesh)
      p.SetId(self.id)
      p.SetGroup(cpp.TRACK)
      self.physics.append(p)
      

  def Release(self):
    self.graphics.Release()
    self.physics.Release()
