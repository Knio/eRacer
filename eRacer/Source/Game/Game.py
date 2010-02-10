'''

A Game object that manages modules

'''

from Core.Globals import *
from Core.Time    import Time
from State        import State

import Core.Globals

class Game(eRacer.Game):
  def __init__(self):
    Core.Globals._set_game(self)
    print Core.Globals.game()
    self.modules = []
    self.namedmodules = {}
    self.time = Time()
    self.state = 0
    self.simspeed = 1.0
    self.states = [State()]
    eRacer.Game.__init__(self)
    
  def PushState(self, state):
    print 'Game::PushState(%r)' % state
    s = self.states[-1]
    if s: s.Deactivate()
    state.parent = s
    self.states.append(state)
    state.Activate()
    
  def PopState(self):
    print 'Game::PopState()'
    s = self.states.pop()
    s.Deactivate()
    return s

  def AddModule(self, module, name=None):
    print 'Adding module %r' % module
    self.modules.append(module)
    if name:
      self.namedmodules[name] = module

  def Init(self):
    print 'Game::Init'
    for i in self.modules:
      i.Init()

  def Run(self):  
    print 'Game::Run'
    self.Start()  
    self.time.Zero()
    while self.state:
      t = self.time.Tick(self.simspeed)
      self.Tick(self.time)
    print "self.state == false"
    self.Quit()

  def Start(self):
    print 'Game::Start'
    for i in self.modules:
      i.Start()
    self.state = 1

  def Tick(self, time, speed=1.0):
    #print 'Game::Tick'
    for i in self.modules:
      i.Tick(time)
      
  def Quit(self):
    print 'Game::Quit'
    for i in self.modules:
      i.Quit()

