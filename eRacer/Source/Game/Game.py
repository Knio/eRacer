'''

A Game object that manages modules

'''

from Core.Time import Time
import Core.Globals
import eRacer

class Game(eRacer.Game):
  def __init__(self):
    Core.Globals._set_game(self)
    print Core.Globals.game()
    self.modules = []
    self.namedmodules = {}
    self.time = Time()
    self.state = 0
    self.simspeed = 1.0
    self.states = [None]
    eRacer.Game.__init__(self)
    
  def PushState(self, state):
    state.parent = self.states[-1]
    self.states.append(state)
    
  def PopState(self):
    return self.states.pop()

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
    self.Quit()

  def Start(self):
    print 'Game::Start'
    for i in self.modules:
      i.Start()
    self.state = 1

  def Tick(self, time, speed=1.0):
    for i in self.modules:
      i.Tick(time)
      
  def Quit(self):
    print 'Game::Quit'
    for i in self.modules:
      i.Quit()

