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
    
    meshes = cpp.VectorMesh()
    
    self.CreateMesh(profile, meshes)
    print "Number of track meshes: ",meshes.size()
    
    for i in xrange(meshes.size()):
      tex   = game().io.LoadTexture('AlphaTrackTexture2.png')
      mat   = game().graphics.graphics.DefaultMaterial()
      mat.disown()
      
      g = MeshNode("track")
      g.Init(cpp.Mesh(meshes[i], mat, tex))
      self.graphics.append(g)
      
      p = cpp.TriMesh()
      p.Init(meshes[i])
      p.SetId(self.id)
      p.SetGroup(cpp.TRACK)
      self.physics.append(p)


  def Release(self):
    self.graphics.Release()
    self.physics.Release()
