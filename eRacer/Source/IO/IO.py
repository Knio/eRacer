from Game.Module  import Module
from Core.Globals import *

import os
import threading
import Queue

j = os.path.join
MODELPATH = 'Resources/Models'
TEXPATH   = 'Resources/Textures'

def asynchronous(func):
  def f(self, callback, *args, **kwargs):
    callback(func(self, *args, **kwargs))
    # self.queue.put_nowait((func, callback, args, kwargs))
  return f
  
def debug(func):
  def f(*args, **kwargs):
    print ('%s(%r, %r) = ' % (func.__name__, args, kwargs)),
    r = func(*args, **kwargs)
    print repr(r)
    return r
  return f

class IO(Module, cpp.IO):
  def __init__(self):
    Module.__init__(self)
    cpp.IO.__init__(self, game().graphics.d3d)
    
    # work queue
    self.queue = Queue.Queue()
    
    # thread
    self.thread = threading.Thread(target=self.work)
    self.thread.setDaemon(True)
    self.thread.start()
    
    # cache
    self.defaulttex = self._LoadTexture(j(TEXPATH, "Default.png"))
    self.textures   = {}
    self.meshes     = {}
    self.args = ()
    
  def work(self):
    while 1:
      func, callback, args, kwargs = self.queue.get()
      r = func(self, *args, **kwargs)
      callback(r)

  def LoadMesh(self, name, *args):
    name = j(MODELPATH,name)
    # print "Loading mesh %s" % name
    if not name in self.meshes:
      mesh = cpp.CachedMesh()
      r = self._LoadMesh(name, mesh)
      if not r:
        print 'Failed to load mesh "%s"' % name
        return None # should throw exception
      print "Loaded mesh %s" % name
      self.meshes[name] = mesh
              
    textures = cpp.VectorTexture()
    cached = self.meshes[name]
    for i in range(cached.nMaterials):
      textures.push_back(self.LoadTexture(cached.texturePatterns[i] % args))

    result = cpp.Mesh()
    result.thisown = 0
    result.Init(cached, textures)

    return result
  
  LoadMeshAsync = asynchronous(LoadMesh)
    
  #@debug
  def LoadTexture(self, name):
    if name:
      name = j(TEXPATH, name)
      # if self.args:
      # name = name % self.args
      # self.args = ()
    if not name in self.textures:
      r = self._LoadTexture(name)
      r.disown()
      if not self.valid(r):
        print 'Failed to load texture "%s", using default' % name
        return self.defaulttex
      print 'Loaded texture %s' % name
      self.textures[name] = r
    return self.textures[name]

  def Loaded(self):
    pass
  LoadAsyncEvent = asynchronous(Loaded)
  
  def Quit(self):
    Module.Quit(self)
    for k,t in self.textures.items():
      self._FreeTexture(t)
    self.textures = {}
    for k,m in self.meshes.items():
      # mesh destroctors will be called below when python's refcounts go to 0
      pass
    self.meshes = {}
