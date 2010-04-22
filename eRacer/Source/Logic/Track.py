from Core.Globals import *

import imp
import pkgutil

class Track(Entity, cpp.Track):
  PATH = "Source/Tracks/"  
  tracks = {}
  def __init__(self, track):
    Entity.__init__(self)
    cpp.Track.__init__(self)
    self.graphics = []
    self.physics  = []
    
    # the fromlist must be non-empty for this to actually assign the whole package to track
    # otherwise only Tracks will be stored in track     
    
    self.name = track.NAME
    self.music = track.MUSIC
    
    for i in track.TRACK:
      self.Add(i)
    
    self.Subdivide(6)
    
    self.dist = self.GetTotalDist()
    
    profile = cpp.VectorTrackVertex()
    for i in track.PROFILE:
      profile.push_back(i)
    
    meshes = cpp.VectorMesh()
    
    self.CreateMesh(profile, meshes, 400)
    print meshes.size()
    
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


        
for importer, modname, ispkg in pkgutil.iter_modules([Track.PATH]):
  track = imp.load_module(modname, *imp.find_module(modname, [Track.PATH]))
  track.NAME = getattr(track, 'NAME', modname)
  Track.tracks[modname] = track
