'''
eRacer game.
'''

from Core.Globals import *

from Game.Game          import Game
from Core.Event         import Event
from Core.Config        import Config
from Logic.MenuState    import MainMenuState

from IO                 import IO
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
    self.graphics  = Graphics(self)
    self.io        = IO(self)
    self.input     = Input(self)
    self.logic     = Logic(self)
    self.sound     = Sound(self)
    self.physics   = Physics(self)
    
    # order that modules will be ticked in the main loop
    self.AddModule(self.io)
    self.AddModule(self.input)
    self.AddModule(self.logic)
    self.AddModule(self.sound)
    self.AddModule(self.physics)
    self.AddModule(self.graphics)
    # if hasattr(eRacer, 'TestModule'):
    #     self.test = eRacer.TestModule();
    
    self.event.Register(self.QuitEvent)
    self.event.Register(self.MouseButtonPressedEvent)
    self.event.Register(self.MouseMovedEvent)
    self.event.Register(self.GameStateChangeEvent)
    
    self.PushState(MainMenuState())    
    
  def Init(self):
    Game.Init(self)

  def Tick(self, time):
    # hack! we need the *current* physics results
    # to compute stable results
    self.physics.physics.GetPhysicsResults()
    
    self.states[-1].Tick(time)
    Game.Tick(self, time)
    

  def MouseButtonPressedEvent(self, mouseButton):
    pass
    # print "Mouse Button ",mouseButton,"pressed"  

  def MouseMovedEvent(self, relativeX, relativeY):
    pass
    # print "Mouse moved by (",relativeX,",",relativeY,")"  
	    
      
  def QuitEvent(self):
    self.state = 0
    
  def GameStateChangeEvent(self, state):
    pass
