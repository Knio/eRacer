from Core.Globals import *

import Track1
import Track2


class Track(Entity, eRacer.Track):
  def __init__(self, scene, name):
    Entity.__init__(self)
    eRacer.Track.__init__(self)
    
    self.physics  = eRacer.TriMesh()
    self.graphics = scene.CreateStaticMeshNode("track")
    
    track = None
    # track = __import__(name)
    if name == 'Track1':
      track = Track1
    if name == 'Track2':
      track = Track2
    
    
    for i in track.TRACK:
      self.Add(i)
    
    self.Subdivide(6)
    
    self.dist = self.GetTotalDist()
    
    profile = eRacer.VectorTrackVertex()
    for i in track.PROFILE:
      profile.push_back(i)
    
    mesh = self.CreateMesh(profile)
    tex  = game().io.LoadTexture('concrete_plates.jpg')
    mat  = game().graphics.graphics.DefaultMaterial()
    mesh.disown()
    mat.disown()
    self.mesh = eRacer.Mesh(mesh, mat, tex)
    
    self.graphics.Init(self.mesh)
    self.physics.Init(mesh)
    self.physics.SetGroup(eRacer.TRACK)
    
  def Tick(self, time):
     Entity.Tick(self, time)

     
