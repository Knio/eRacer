from Game.Module import Module
import eRacer

import os
import threading
import Queue

j = os.path.join
MODELPATH = 'Resources/Models'
TEXPATH   = 'Resources/Textures'

def asynchronous(func):
  def f(self, callback, *args, **kwargs):
    self.queue.put_nowait((func, callback, args, kwargs))
  return f
  
def debug(func):
  def f(*args, **kwargs):
    print ('%s(%r, %r) = ' % (func.__name__, args, kwargs)),
    r = func(*args, **kwargs)
    print repr(r)
    return r
  return f

class IO(Module, eRacer.IO):
  def __init__(self, game):
    Module.__init__(self, game)
    eRacer.IO.__init__(self, game.graphics.d3d)
    
    # work queue
    self.queue = Queue.Queue()
    
    # thread
    self.thread = threading.Thread(target=self.work)
    self.thread.setDaemon(True)
    self.thread.start()
    
    # cache
    self.defaulttex = self._LoadTexture(j(TEXPATH, "Default.png"))
    self.textures = {}
    self.meshes   = {}
    
  def work(self):
    while 1:
      func, callback, args, kwargs = self.queue.get()
      r = func(self, *args, **kwargs)
      callback(r)

  def LoadMesh(self, node, name):
    name = j(MODELPATH,name)
    if not name in self.meshes:
      r = self._LoadMesh(name)
      if not self.valid(r):
        print 'Failed to load mesh "%s"' % name
        return -1
      self.meshes[name] = r

    mesh = self.meshes[name]    
    self._SetMesh(node, mesh)
    return 0
  
  LoadMeshAsync = asynchronous(LoadMesh)
    
  #@debug
  def LoadTexture(self, name):
    if name:
      name = j(TEXPATH, name)
    if not name in self.textures:
      r = self._LoadTexture(name)
      r.disown()
      if not self.valid(r):
        print 'Failed to load texture "%s", using default' % name
        return self.defaulttex
      self.textures[name] = r
    
    return self.textures[name]
    
  def Quit(self):
    Module.Quit(self)
    for k,t in self.textures.items():
      self._FreeTexture(t)
    self.textures = {}
    for k,m in self.meshes.items():
      self._FreeMesh(m)
    self.meshes = {}