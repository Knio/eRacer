import threading


from Core.Globals   import *
from Game.State     import State

from GameMapping    import GameMapping
from MenuState      import PauseMenuState

# Entities
from Box        import Box
from Plane      import Plane
from Track      import Track
from Ship       import Ship
from Vehicle    import Vehicle
from Camera     import ChasingCamera, FirstPersonCamera, CarCamera
from Starfield  import Starfield
from Meteor     import Meteor, MeteorManager
# from CoordinateCross  import CoordinateCross


# AI stuff
from AI.Behavior import PlayerBehavior, AIBehavior
from AI.Raceline import Raceline

# View stuff
from Graphics.View    import View
from Graphics.SkyBox  import SkyBox

from Graphics.Sprite  import Sprite


# TODO
# need a lock so that loading of IO does not happen
# inside BegineScene()/EndScene() pairs, or else the 
# driver will freak out
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
    State.Activate(self)
    #if not self.loaded:
    #  game().PushState(LoadingState(self.load))
    print "Activate game state"

  def Deactivate(self):
    State.Deactivate(self)    
    # self.sound.isPaused = True
    # game().sound.sound.UpdateSoundFx(self.sound)
    
    
  def load(self):
    # testing stuff
    # game().sound.PlaySound2D("jaguar.wav")
    print "GameState::load begin"

    # TODO
    # can we render a fake loading screen here until the real one works?
    lineData = [Point3(80, 2, 20), Point3(80, 0, 100)]
    raceline = Raceline(lineData)
    
    scene = eRacer.Scene()
    self.scene = scene
        
    self.player = Vehicle(self.scene)
    self.player.behavior = PlayerBehavior(self.player)
  
    # self.ai1 = Vehicle(self.scene, Vector3(80, 2, -20))
    # self.ai1.behavior = AIBehavior(self.ai1, raceline)
    

    game().logic.Add(self.player)
    # game().logic.Add(self.ai1)
    
    

    self.views = []
    self.viewIndex = 0
    
    cam = game().logic.Add(ChasingCamera(self.player))
    self.views.append(View(cam)) #eRacer.View(self.scene, cam.camera))
    
    cam = game().logic.Add(FirstPersonCamera())
    self.views.append(View(cam)) #eRacer.View(self.scene, cam.camera))
    
    cam = game().logic.Add(CarCamera(self.player))
    self.views.append(View(cam)) #eRacer.View(self.scene, cam.camera))
    
    
    
    # without this, the skyboxes are garbage collected because the 
    # reference in view does not count because view is a c++ object (not python)
    self.skybox = SkyBox()

    self.starfields = [
      game().logic.Add(Starfield(1024, 1000.0)),
      game().logic.Add(Starfield(1024, 100.0)),
      game().logic.Add(Starfield(1024, 20.0)),    
    ]
    
    for view in self.views:
      view.AddRenderable(self.scene)
      for s in self.starfields:
        view.AddRenderable(s)      
      view.AddRenderable(self.skybox)
      
    game().logic.Add(Ship(scene))
    game().logic.Add(Track(scene))
    
    
    # game().logic.Add(Plane(scene))
    # self.coordinatecross = CoordinateCross(self.view)
    # game().logic.Add(self.coordinatecross)
    
    #for j in range (24, 35, 3):
    #  for i in range(-191, -225, -2):
    #    game().logic.Add(Box(scene, Vector3(-1850, j, i)))
        
    # for i in range(-191, -230, -5):
    #     game().logic.Add(Box(scene, Vector3(-1850, 24, i)))
    # for i in range(-193, -230, -5):
    #     game().logic.Add(Box(scene, Vector3(-1850, 26, i)))
    # for i in range(-195, -235, -5):
    #     game().logic.Add(Box(scene, Vector3(-1850, 28, i)))
    # for i in range(-193, -230, -5):
    #     game().logic.Add(Box(scene, Vector3(-1850, 30, i)))
    # for i in range(-191, -230, -5):
    #     game().logic.Add(Box(scene, Vector3(-1850, 32, i)))
        
    self.meteorManager = MeteorManager(self.scene)
    game().logic.Add(self.meteorManager)

    for i in range(1,100):
      m = self.meteorManager.spawnRandom()
      game().logic.Add(m)
    
    self.lastMeteorTime = 0
    
      
    # self.sound = eRacer.SoundFx();
    # self.sound.looping  = True
    # self.sound.is3D     = False
    # self.sound.isPaused = False
    # game().sound.sound.LoadSoundFx("Resources/Sounds/Adventure.mp3", self.sound)
    
    
    game().time.Zero()
    self.loaded = True
    
  def get_view(self):
    return self.views[self.viewIndex]
    
  view = property(get_view)
  
  
  AIMED_METEOR_INTERVAL = 5.
    
  def Tick(self, time):
    # int SetOrientation3D(const Point3& listenerPos, const Vector3& listenerVel, const Vector3& atVector, const Vector3& upVector); //For 3D sound
    cam = self.view.camera
    game().sound.sound.SetOrientation3D(cam.GetPosition(), Point3(0,0,0), cam.GetLookAt(), cam.GetUp())
    
    
    
    State.Tick(self, time)
    game().graphics.views.append(self.view)
    
    self.lastMeteorTime += time.game_delta
    if self.lastMeteorTime > self.AIMED_METEOR_INTERVAL*time.RESOLUTION:
      self.lastMeteorTime = 0
      m = self.meteorManager.spawnAimed(eRacer.ExtractPosition(self.player.transform))
      game().logic.Add(m)
  
  

      
  def CameraChangedEvent(self):
    self.viewIndex = (self.viewIndex+1) % len(self.views)
    
  def ReloadConstsEvent(self):
    game().config.read()
    game().event.ReloadedConstsEvent()
    
  def PauseEvent(self):
    game().PushState(PauseMenuState())

  def PlayJaguarSoundEvent(self):
    game().sound.PlaySound2D("jaguar.wav")

