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

import eRacer

# testing entities
from Logic.Box   import Box
from Logic.Plane import Plane
from Logic.Ship  import Ship


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
    if hasattr(eRacer, 'TestModule'):
        self.test = eRacer.TestModule();
    
    self.event.Register(self.QuitEvent)
    self.event.Register(self.KeyPressedEvent)
    
    
  def Init(self):
    Game.Init(self)

    # testing stuff
    self.sound.PlaySound2D("jaguar.wav")
    
    # space ship
    self.logic.Add(Ship(self))
    self.logic.Add(Plane(self))
    self.boxcount = 0
    
    # camera
    from Logic.Camera import Camera
    camera = Camera(self)
    self.logic.Add(camera)
    self.graphics.SetCamera(camera)    

    
  def Tick(self, time):
    Game.Tick(self, time) 
    
    if time.seconds > self.boxcount:
      self.boxcount += 1
      self.logic.Add(Box(self))   
    
  def KeyPressedEvent(self, key):
    from Input import KEY
    if key == KEY.SPACE:
      self.sound.PlaySound2D("jaguar.wav")
    
    if key == KEY.ESCAPE:
      self.event.QuitEvent()   
      
      
  def QuitEvent(self):
    self.state = 0