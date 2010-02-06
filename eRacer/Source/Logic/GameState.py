from Core.Globals   import *
from Game.State     import State

from MenuState  import PauseMenuState
from Box        import Box
from Plane      import Plane
from Track      import Track
from Ship       import Ship
from Vehicle    import Vehicle
from Starfield  import Starfield
from Camera     import ChasingCamera
from GameMapping    import GameMapping

class GameState(State):
  MAPPING = GameMapping
  def __init__(self):
    State.__init__(self)
    self.load()
    
  def load(self):
    # testing stuff
    game().sound.PlaySound2D("jaguar.wav")
    
    scene = eRacer.Scene()
    self.scene = scene
    game().graphics.graphics.m_scene = scene
    
    # TODO this should be in some 
    # map-loading code with a progress bar
    self.scene.LoadSkyBox('skybox2.x')
    
    
    self.player = Vehicle(self.scene)
    self.camera = ChasingCamera(self.player)
    
    game().logic.Add(self.player)
    game().logic.Add(self.camera)
    
    
    
    game().logic.Add(Ship(scene))
    game().logic.Add(Track(scene))
    game().logic.Add(Plane(scene))    
    
    game().logic.Add(Starfield(scene, self.camera, 1024, 1000.0))
    game().logic.Add(Starfield(scene, self.camera, 1024,  100.0))
    game().logic.Add(Starfield(scene, self.camera, 1024,   20.0))
    

    self.boxcount = 1
    game().time.Zero()
    
    
  def Tick(self, time):
    State.Tick(self, time)

    game().graphics.scene  = self.scene
    game().graphics.camera = self.camera
    #game().graphics.views.append(self.view)
    
    if time.seconds > self.boxcount:
      self.boxcount += max(self.boxcount+1, 20)
      game().logic.Add(Box(self.scene))
      
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