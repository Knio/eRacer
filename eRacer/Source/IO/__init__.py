from Game import Module
import eRacer

import os
import threading
import Queue

j = os.path.join
MODELPATH = 'Resources/Models'

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
    self.defaulttex = eRacer.IO.LoadTexture(self, "Default.png")
    self.textures = {}
    
  def work(self):
    while 1:
      func, callback, args, kwargs = self.queue.get()
      r = func(self, *args, **kwargs)
      callback(r)

  @asynchronous
  def LoadMeshAsync(self, node, name):
    return self.LoadMesh(node, j(MODELPATH,name))
    
  #@debug
  def LoadTexture(self, name):
    if not name in self.textures:
      r = eRacer.IO.LoadTexture(self, name)
      r.disown()
      if not self.valid(r):
        print 'Failed to load texture "%s", using default' % name
        return self.defaulttex
      self.textures[name] = r
    
    return self.textures[name]