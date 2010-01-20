from Game import Module

import threading
import Queue

def asynchronous(func):
  def f(self, callback, *args, **kwargs):
    self.queue.put_nowait(func, callback, args, kwargs)
  return f

class IO(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    self.queue = Queue.Queue()
    self.thread = threading.Thread(target=self.work)
    self.thread.setDaemon(True)
    self.thread.start()

  def work(self):
    while 1:
      func, callback, args, kwargs = self.queue.get()
      r = func(self, *args, **kwargs)
      callback(r)

  @asynchronous
  def LoadMesh(self, name):
    pass