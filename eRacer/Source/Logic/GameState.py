import threading
import time as _time

from Core.Globals   import *
from Game.State     import State

from GameMapping    import GameMapping
from MenuState      import PauseMenuState
from GameEndState   import GameEndState

# Entities
from Track      import Track
from Vehicle    import Vehicle
from Shadow     import Shadow
from Camera     import ChasingCamera, FirstPersonCamera, CarCamera, OrthographicCamera
from Starfield  import Starfield
from Meteor     import Meteor, MeteorManager
from Quad  import Quad
from HudQuad  import HudQuad
# from CoordinateCross  import CoordinateCross


# AI stuff
from AI.Behavior import PlayerBehavior, AIBehavior
from AI.Raceline import Raceline

# View stuff
from Graphics.View    import View
from Graphics.SkyBox  import SkyBox




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
  def __init__(self, track='Track1'):
    State.__init__(self)
    self.loaded = False
    
    self.laps   = 2 # TODO CONST
    self.stats  = {}
    self.gameOver = False
    
    self.load(track)
    
  def Activate(self):
    State.Activate(self)
    #if not self.loaded:
    #  game().PushState(LoadingState(self.load))
    print "Activate game state"

  def Deactivate(self):
    State.Deactivate(self)    
    # self.sound.isPaused = True
    # game().sound.sound.UpdateSoundFx(self.sound)
    
    
  def load(self, track):
    # testing stuff
    # game().sound.PlaySound2D("jaguar.wav")
    print "GameState::load begin"
    scene = cpp.Scene()
    self.scene = scene
    
    # TODO
    # can we render a fake loading screen here until the real one works?
    
    self.track = self.Add(Track(track))
    
    frame = self.track.GetFrame(-30.0)
    frametx = Matrix(frame.position, frame.up, frame.fw)
    
    forwardMat = Matrix(ORIGIN, -PI/2.0, X)
    
    self.player = self.Add(Vehicle("Player", self.track, 
      Matrix(Point3(0, 3, 0)) * frametx,
    ))
    PlayerBehavior(self.player)
    self.Add(Shadow(self.player))
    
    self.AddAICar("AI1", Matrix(Point3(-15, 3, 0)) * frametx, 2)
    self.AddAICar("AI2", Matrix(Point3(+15, 3, 0)) * frametx, 5)
    self.AddAICar("AI3", Matrix(Point3(0, 3, -15)) * frametx, 2)
    self.AddAICar("AI4", Matrix(Point3(-15, 3, -15)) * frametx, 5)
    self.AddAICar("AI5", Matrix(Point3(+15, 3, -15)) * frametx, 2)
    self.AddAICar("AI6", Matrix(Point3(0, 3, -30)) * frametx, 5)
    self.AddAICar("AI7", Matrix(Point3(-15, 3, -30)) * frametx, 2)
    self.AddAICar("AI8", Matrix(Point3(+15, 3, -30)) * frametx, 5)

    startFrame = self.track.GetFrame(0.0)
    
    # TODO: this should load "StartLine.x" but it is not appearing properly
    finishLineTransform = Matrix(30, 1, 3) * Matrix(startFrame.position+startFrame.up, startFrame.up, startFrame.fw)
    self.Add(Model('Finish Line','FinishLine.x',None,finishLineTransform))
    
    def CarTrackCollisionEvent(car, track, force):
      pass
      # print 'CAR-TRACK:', car, track, force
      # if length(force):
      #   game().simspeed = 0.0
      
    game().event.Register(CarTrackCollisionEvent)

    self.views = []
    self.viewIndex = 0
    
    cam = self.Add(ChasingCamera(self.player))
    self.views.append(View(cam)) #eRacer.View(self.scene, cam.camera))
    
    cam = self.Add(FirstPersonCamera())
    self.views.append(View(cam)) #eRacer.View(self.scene, cam.camera))
    
    cam = self.Add(CarCamera(self.player))
    self.views.append(View(cam)) #eRacer.View(self.scene, cam.camera))    
    
    # need refactoring
    self.hudView = View(OrthographicCamera(game().window.width,game().window.height))

    # 0,0 is top left, make sure you add all HudQuads using AddHud
    # texture coordinates can be set via self.boostBar.graphics.SetTextureCoordinates
    # wrappers for that should be created as needed. 
    # self.boostBar = self.AddHud(HudQuad("BoostBar", "eRacerXLogoNegative.png", 0, 0, 600, 235))

    self.skybox = SkyBox()
    
    
    for view in self.views:
      view.AddRenderable(self.scene)
      
      view.AddRenderable(Starfield(1024, 1000.0, view.camera))
      view.AddRenderable(Starfield(1024, 100.0,  view.camera))
      view.AddRenderable(Starfield(1024, 20.0,   view.camera))
      
      view.AddRenderable(self.skybox)
   
    self.meteorManager = MeteorManager(self)
    self.Add(self.meteorManager)

    for i in range(CONSTS.NUM_METEORS):
      m = self.meteorManager.spawnRandom()
      self.Add(m)
    
    self.lastMeteorTime = 0
    
    self.sound = cpp.SoundFx();
    self.sound.looping  = True
    self.sound.is3D     = False
    self.sound.isPaused = False
    game().sound.sound.LoadSoundFx("Adventure.mp3", self.sound)
    
    game().time.Zero()
    self.loaded = True
    
  def get_view(self):
    return self.views[self.viewIndex]
    
  view = property(get_view)
  
  
  AIMED_METEOR_INTERVAL = 2.
    
  def Tick(self, time):
    # int SetOrientation3D(const Point3& listenerPos, const Vector3& listenerVel, const Vector3& atVector, const Vector3& upVector); //For 3D sound
    cam = self.view.camera
    # TODO camera velocity
    game().sound.sound.SetOrientation3D(cam.GetPosition(), Point3(0,0,0), cam.GetLookAt(), cam.GetUp())
    
    _time.sleep(CONSTS.SLEEP_TIME)
    
    
    game().graphics.views.append(self.view)
    game().graphics.views.append(self.hudView)
    

    game().graphics.graphics.WriteString( "BOOST %2.2f" % (self.player.boostFuel), "Verdana", 50, Point3(250,500,0))
    
    if self.player.lapcount:
      playerLaps = min(self.player.lapcount, self.laps)
      
      game().graphics.graphics.WriteString("%d" % (playerLaps), "Sony Sketch EF",96, Point3(650, 0, 0))
      game().graphics.graphics.WriteString("/", "Sony Sketch EF", 80, Point3(690, 20, 0))
      game().graphics.graphics.WriteString("%d" % (self.laps), "Sony Sketch EF", 80, Point3(720, 30, 0))
    
      l = list(self.stats.get(self.player,[0.]))
      l.append(game().time.get_seconds())
      
      y = 100
      for i,t in enumerate(l):
        if not i or i>self.laps: continue
        game().graphics.graphics.WriteString("Lap %d:" % i, "Sony Sketch EF", 24, Point3(650, y, 0))
        game().graphics.graphics.WriteString("%05.2f"   % (t-l[i-1]), "Sony Sketch EF", 24, Point3(720, y, 0))
        y += 15    
    
    if not self.gameOver:
      self.lastMeteorTime += time.game_delta
      if self.lastMeteorTime > self.AIMED_METEOR_INTERVAL*time.RESOLUTION:
        self.lastMeteorTime = 0
        m = self.meteorManager.spawnTargeted(self.player)
        self.Add(m)
    
    self.meteorManager.Tick(time)
    
    State.Tick(self, time)

  
  def AddHud(self, entity):
    self.entities.append(entity)
    g = getattr(entity, 'graphics', None)
    if g: self.hudView.AddRenderable(g)
    return entity    
  
  def LapEvent(self, vehicle, lap):
    self.stats.setdefault(vehicle, []).append(game().time.get_seconds())
    
    if lap == self.laps+1:
      if vehicle == self.player:
        self.gameOver = True
        game().PushState(GameEndState(self.stats))
        
      vehicle.Brake(1)
        
  def RespawnCarEvent(self):
    self.player.resetCar()
      
  def CameraChangedEvent(self):
    self.viewIndex = (self.viewIndex+1) % len(self.views)
    
  def ReloadConstsEvent(self):
    game().config.read()
    game().event.ReloadedConstsEvent()
    
  def PauseEvent(self):
    game().PushState(PauseMenuState())

  def PlayJaguarSoundEvent(self):
    game().sound.PlaySound2D("jaguar.wav")
    
  def KeyPressedEvent(self, key):
    if key == KEY.HOME:
      game().simspeed = 1.0
      
  def AddAICar(self, name, orient, modelNum):
      ai    = self.Add(Vehicle(name,    self.track, 
      orient, modelNum))
      AIBehavior(ai, self.track)
      self.Add(Shadow(ai))

