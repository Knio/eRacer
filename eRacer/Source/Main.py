'''
eRacer game.
'''

import os
# set path to load fmod.dll
os.environ['PATH'] += ';Lib'

from Game     import Game, Entity
from Core     import Event
from IO       import IO
from Input    import Input
from Logic    import Logic
from Sound    import Sound
from Graphics import Graphics
from Physics  import Physics

class TestEntity(Entity):
  def Tick(self, time):
    pass
    #print 'Hi! %10r %10r %10r' % (time.elapsed, time.delta, time.Fps())

class Main(Game):
  def __init__(self):
    Game.__init__(self)
    
    self.event = Event(self)
    
    # graphics must be created first because
    # some other modules need a HWND
    self.io        = IO(self) 
    self.graphics  = Graphics(self)
    self.input     = Input(self)
    self.logic     = Logic(self)
    self.sound     = Sound(self)
    self.physics   = Physics(self)
    
    # order that modules will be ticked in the main loop
    self.AddModule(self.input)
    self.AddModule(self.logic)
    self.AddModule(self.sound)
    self.AddModule(self.physics)
    self.AddModule(self.graphics)
    
    # testing stuff
    self.logic.Add(TestEntity())
    self.sound.PlaySound2D("Resources/jaguar.wav")
    
    self.event.Register('QuitEvent', self)
    self.event.Register('KeyPressedEvent', self)
    
    
  def Tick(self, time):
    Game.Tick(self, time)    
    
  def KeyPressedEvent(self, key):
    from Input import KEY
    if key == KEY.SPACE:
      self.sound.PlaySound2D("Resources/jaguar.wav")
    
    if key == KEY.ESCAPE:
      self.event.QuitEvent()
      
  def QuitEvent(self):
    self.state = 0