import threading

from Core.Globals   import *
from Game.State     import State

from MenuState  import PauseMenuState
from Box        import Box
from Plane      import Plane
from Track      import Track
from Ship       import Ship
from Vehicle    import Vehicle
from Camera     import ChasingCamera, FirstPersonCamera
from Starfield  import Starfield
from Graphics.SkyBox     import SkyBox

from CoordinateCross import CoordinateCross
from GameMapping    import GameMapping


class LoadingState(State):
  def __init__(self, func):
    State.__init__(self)    
        
    def load():
      # do loading function
      func()
      # wait for all asyncronous loads to finish
      game().io.LoadAsyncEvent(self.Loaded)
    
    self.thread = threading.Thread(target=load)
    self.thread.start()
    
  def Activate(self):
    game().simspeed = 0.
  
  def Deactivate(self):
    game().simspeed = 1.    
    
  def Loaded(self, none):
    game().PopState()
    
  def Tick(self, time):
    State.Tick(self, time)
    game().graphics.graphics.WriteString(
      "Loading...", 
      "Verdana", 32, Point3(300,220,0)
    )    

class GameState(State):
  MAPPING = GameMapping
  def __init__(self):
    State.__init__(self)
    self.loaded = False
    self.load()
    
  def Activate(self):
    #if not self.loaded:
    #  game().PushState(LoadingState(self.load))
    print "Activate game state"
    pass
    
  def load(self):
    # testing stuff
    # game().sound.PlaySound2D("jaguar.wav")
    print "GameState::load begin"

    scene = eRacer.Scene()
    self.scene = scene
        
    self.player = Vehicle(self.scene)
    
    # OK
    game().logic.Add(self.player)
    
    self.views = []
    
    cam = ChasingCamera(self.player)
    game().logic.Add(cam)
    self.views.append(eRacer.View(self.scene, cam.camera))
    
    cam = FirstPersonCamera()
    game().logic.Add(cam)
    self.views.append(eRacer.View(self.scene, cam.camera))
    
    self.viewIndex = 0
    
    game().logic.Add(self.player)
    
    # is garbage collected if i dont store - why???
    self.skyboxes = []


    for view in self.views:
      game().logic.Add(Starfield(view, 1024, 1000.0))
      game().logic.Add(Starfield(view, 1024, 100.0))
      game().logic.Add(Starfield(view, 1024, 20.0))
      self.skyboxes.append(SkyBox(view))
    
    
    ship = Ship(scene)
    print "ship created"
    
    game().logic.Add(ship)
    print "ship added"
    game().logic.Add(Track(scene))
    game().logic.Add(Plane(scene))
    
    
    # self.coordinatecross = CoordinateCross(self.view)
    # game().logic.Add(self.coordinatecross)
    
    game().time.Zero()
    self.loaded = True
    print "GameState::load end"
    
  def get_view(self):
    return self.views[self.viewIndex]
    
  view = property(get_view)   
    
  def CameraChangedEvent(self):
    # print "Camera ",self.cameraIndex+1," out of ",len(self.cameras)
    self.viewIndex+=1
    if(self.viewIndex>=len(self.views)): self.viewIndex=0 
    
  def Tick(self, time):
    print "GameState::Tick"
    State.Tick(self, time)

    #game().graphics.scene  = self.scene
    #game().graphics.camera = self.camera
    game().graphics.views.append(self.view)
    
    # if time.seconds > self.boxcount:
    #   self.boxcount += min(self.boxcount+1, 20)
    #   game().logic.Add(Box(self.scene))
      
  def PauseEvent(self):
    game().PushState(PauseMenuState())

  def KeyPressedEvent(self, key):   
    if key == KEY.SPACE:
      game().sound.PlaySound2D("jaguar.wav")          
    
    if key == KEY.R:
      game().config.read()
      game().event.ReloadConstsEvent()        
      
  def GamepadButtonPressedEvent(self, button):
    if button == eRacer.BUTTON_START:
      game().PushState(PauseMenuState())