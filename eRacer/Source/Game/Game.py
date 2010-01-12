'''

A Game object that manages modules

'''

from Core import Time

class Game(object):
  def __init__(self):
    self.modules = []
    self.namedmodules = {}
    self.time = Time()
    self.state = 0

  def addmodule(self, module, name=None):
    self.modules.append(module)
    if name:
      self.namedmodules[name] = module

  def __getattr__(self, name):
    '''
    getter for named modules
    '''
    try:
      return self.namedmodules[name]
    except KeyError: raise AttributeError

  def start(self):
    for i in self.modules:
      i.start()
    self.state = 1

  def run(self):  
    self.start()  
    while self.state:
      t = self.time.Tick()
      self.tick(self.time)
    self.quit()

  def tick(self, time):
    for i in self.modules:
      i.tick(time)
      
  def quit(self):
    for i in self.modules:
      i.quit()

