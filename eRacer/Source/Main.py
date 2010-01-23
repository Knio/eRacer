'''
eRacer game.
'''

import os

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
    # quick fps counter
    #print 'Hi! %10r %10r %10r' % (time.elapsed, time.delta, time.Fps())

class Main(Game):
  def __init__(self):
    Game.__init__(self)
    
    self.event = Event(self)
    
    # graphics must be created first because
    # some other modules need the HWND or D3DDEVICE
    
    self.graphics  = Graphics(self)
    self.io        = IO(self)
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
    
    self.event.Register(self.QuitEvent)
    self.event.Register(self.KeyPressedEvent)
    
    
    # testing stuff
    self.logic.Add(TestEntity(self))
    self.sound.PlaySound2D("jaguar.wav")
    
    # space ship
    from Logic.Box   import Box
    from Logic.Plane import Plane
    from Logic.Ship  import Ship
    
    self.logic.Add(Ship(self))
    self.logic.Add(Box(self))
    self.logic.Add(Plane(self))
    
    
    # camera
    from Logic.Camera import Camera
    camera = Camera(self)
    self.logic.Add(camera)
    self.graphics.SetCamera(camera)
    
  def Tick(self, time):
    Game.Tick(self, time)    
    
  def KeyPressedEvent(self, key):
    from Input import KEY
    if key == KEY.SPACE:
      self.sound.PlaySound2D("jaguar.wav")
    
    if key == KEY.ESCAPE:
      self.event.QuitEvent()
      
  def QuitEvent(self):
    self.state = 0