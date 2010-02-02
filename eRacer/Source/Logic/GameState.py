from Core.Globals   import *
from Game.State     import State

from MenuState  import PauseMenuState
from Box        import Box
from Plane      import Plane
from Track      import Track
from Ship       import Ship
from Vehicle    import Vehicle
from Camera     import ChasingCamera

class GameState(State):
  def __init__(self):
    State.__init__(self)
        
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

    self.boxcount = 1
    game().time.Zero()
    
    
  def Tick(self, time):
    State.Tick(self, time)
    print time.game_delta
    game().graphics.scene  = self.scene
    game().graphics.camera = self.camera
    #game().graphics.views.append(self.view)
    
    if time.seconds > self.boxcount:
      self.boxcount += 1
      game().logic.Add(Box(self.scene))
      
    
  def KeyPressedEvent(self, key):
    if key == KEY.W:  game().event.PlayerAccelerateEvent(+1);
    if key == KEY.S:  game().event.PlayerAccelerateEvent(-1);
    if key == KEY.A:  game().event.PlayerTurnEvent(-1);
    if key == KEY.D:  game().event.PlayerTurnEvent(+1);
    
    if key == KEY.ESCAPE:
      game().PushState(PauseMenuState())

    if key == KEY.SPACE:
      game().sound.PlaySound2D("jaguar.wav")          
    
    if key == KEY.R:
      game().config.read()
      game().event.ReloadConstsEvent()    

