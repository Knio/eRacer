'''
eRacer game.
'''

from Core.Globals import *

from Game     import Game
from Core     import Event
from Core     import Config

from IO       import IO
from Input    import Input
from Logic    import Logic
from Sound    import Sound
from Graphics import Graphics
from Physics  import Physics

# testing entities
from Logic.Box      import Box
from Logic.Plane    import Plane
from Logic.Ship     import Ship
from Logic.Vehicle  import Vehicle
from Logic.Track	import Track

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
    if hasattr(eRacer, 'TestModule'):
        self.test = eRacer.TestModule();
    
    self.event.Register(self.QuitEvent)
    self.event.Register(self.KeyPressedEvent)
    self.event.Register(self.MouseButtonPressedEvent)
    self.event.Register(self.MouseMovedEvent)
    
    
  def Init(self):
    Game.Init(self)

    # testing stuff
    self.sound.PlaySound2D("jaguar.wav")
    
    # space ship
    self.logic.Add(Ship(self))
    self.logic.Add(Track(self))
    self.boxcount = 5
    
    vehicle = Vehicle(self)
    # car
    self.logic.Add(vehicle)
    
    # camera
    from Logic.Camera import FirstPersonCamera
    camera = FirstPersonCamera(self)
    self.logic.Add(camera)
    self.graphics.SetCamera(camera)    

    
  def Tick(self, time):
    #self.simspeed = 0.2
    #_time.sleep(0.02)
    
    # hack! we need the *current* physics results
    # to compute stable results
    self.physics.physics.GetPhysicsResults()
    
    
    if time.seconds > self.boxcount:
      self.boxcount += 1
      self.logic.Add(Box(self))   
      
    Game.Tick(self, time)      
    
  def KeyPressedEvent(self, key):
    from Input import KEY
    if key == KEY.SPACE:
      self.sound.PlaySound2D("jaguar.wav")
    
    if key == KEY.ESCAPE:
      self.event.QuitEvent()   
      
    if key == KEY.R:
      self.config.read()
      self.event.ReloadConstsEvent()
      
    #if key == KEY.SPACE:
    #  self.logic.Add(Box(self))   

  def MouseButtonPressedEvent(self, mouseButton):
	pass
	# print "Mouse Button ",mouseButton,"pressed"  
	
  def MouseMovedEvent(self, relativeX, relativeY):
	pass
	# print "Mouse moved by (",relativeX,",",relativeY,")"  
	    
      
  def QuitEvent(self):
    self.state = 0
