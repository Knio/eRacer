'''
eRacer game.
'''

from Core.Globals import *

from Game.Game          import Game
from Core.Event         import Event
from Core.Config        import Config
from Logic.MenuState    import MainMenuState

from IO.IO              import IO
from Input.Input        import Input
from Logic.Logic        import Logic
from Sound.Sound        import Sound
from Graphics.Graphics  import Graphics
from Physics.Physics    import Physics

import time as _time

class Main(Game):
  def __init__(self):
    Game.__init__(self)
    self.config = Config()
    self.event  = Event(self)
    
    # graphics must be created first because
    # some other modules need the HWND or D3DDEVICE
    self.graphics  = Graphics()
    self.io        = IO()
    self.input     = cpp.Input()
    self.sound     = Sound()
    self.physics   = Physics()
    
    # order that modules will be ticked in the main loop
    self.AddModule(self.io)
    self.AddModule(self.input)
    self.AddModule(self.sound)
    self.AddModule(self.physics)
    self.AddModule(self.graphics)
    if hasattr(cpp, 'TestModule'):
        self.test = cpp.TestModule();
    
    self.event.Register(self.QuitEvent)
    
  def Init(self):
    Game.Init(self)

  def Tick(self, time):
    # hack! we need the *current* physics results
    # to compute stable results
    self.physics.physics.GetPhysicsResults()
    
    self.states[-1].Tick(time)
    Game.Tick(self, time)
          
  def QuitEvent(self):
    self.state = 0

