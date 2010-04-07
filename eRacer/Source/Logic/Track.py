from Core.Globals import *

import Track1
import Track2

class Track(Entity, cpp.Track):
  def __init__(self, name):
    Entity.__init__(self)
    cpp.Track.__init__(self)
    
    self.graphics = []
    self.physics  = []
    
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
    
    for i in xrange(meshes.size()):
      tex   = game().io.LoadTexture('AlphaTrackTexture2.png')
      mat   = game().graphics.graphics.DefaultMaterial()
      mat.disown()
      meshes[i].disown()
      
      g = MeshNode("Track Segment %d" % i)
      m = cpp.Mesh(meshes[i], mat, tex)
      m.thisown = 0
      g.Init(m)
      self.graphics.append(g)


      p = cpp.TriMesh()
      p.Init(meshes[i])
      p.SetId(self.id)
      p.SetGroup(cpp.TRACK)
      self.physics.append(p)

      
      # print "initializing mesh ",meshes[i]


  def Release(self):
    for g in self.graphics:
      g.Release()
    for p in self.physics:
      p.Release()
