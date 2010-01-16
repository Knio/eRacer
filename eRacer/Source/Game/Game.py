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

  def AddModule(self, module, name=None):
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

  def Run(self):  
    self.Start()  
    while self.state:
      t = self.time.Tick()
      self.Tick(self.time)
    self.Quit()

  def Start(self):
    for i in self.modules:
      i.Start()
    self.state = 1

  def Tick(self, time):
    for i in self.modules:
      i.Tick(time)
      
  def Quit(self):
    print 'Quitting game'
    for i in self.modules:
      i.Quit()

