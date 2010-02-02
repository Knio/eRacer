from Core.Globals import *

# testing entities
from Logic.Box      import Box
from Logic.Plane    import Plane
from Logic.Ship     import Ship
from Logic.Vehicle  import Vehicle
from Logic.Camera   import ChasingCamera, CirclingCamera

class State(eRacer.State):
  def __init__(self):
    pass
    
  def Tick(self, time):
    pass
    
  def KeyPressedEvent(self, key): 
    if key == KEY.SPACE:
      game().sound.PlaySound2D("jaguar.wav")
    
    if key == KEY.ESCAPE:
      game().event.QuitEvent()   
      
    if key == KEY.R:
      game().config.read()
      game().event.ReloadConstsEvent()    
    
    
    
class MenuState(State):
  def __init__(self):
    State.__init__(self)
    self.scene  = eRacer.Scene()
    self.camera = CirclingCamera()
    
  def Tick(self, time):
    State.Tick(self, time)
    
    game().graphics.graphics.WriteString("Push ENTER to begin a new game", "Verdana", 40, Point3(100,100,0))
    
    game().graphics.scene  = self.scene
    game().graphics.camera = self.camera
    
  
class MainMenuState(MenuState):
  def __init__(self):
    MenuState.__init__(self)
    
  def KeyPressedEvent(self, key):
    if key == KEY.RETURN:
      print 'GameState!!'
      game().PushState(GameState())
      print 2
    
  
class PauseMenuState(MenuState): pass
class StatsOverlayState(State): pass
class LoadingState(State):  pass

  
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
    game().logic.Add(Plane(scene))

    self.boxcount = 1
    
    
  def Tick(self, time):
    State.Tick(self, time)
        
    game().graphics.scene  = self.scene
    game().graphics.camera = self.camera
    #game().graphics.views.append(self.view)
    
    if time.seconds > self.boxcount:
      self.boxcount += 1
      game().logic.Add(Box(self.scene))
      
    
  def KeyPressedEvent(self, key):
    State.KeyPressedEvent(self, key)
    if key == KEY.W:  game().event.PlayerAccelerateEvent(+1);
    if key == KEY.S:  game().event.PlayerAccelerateEvent(-1);
    if key == KEY.A:  game().event.PlayerTurnEvent(-1);
    if key == KEY.D:  game().event.PlayerTurnEvent(+1);
    
    #if key == KEY.ESCAPE:
    #  game().state = MenuState()
          
    
    
