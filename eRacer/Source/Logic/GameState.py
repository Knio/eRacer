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
from Meteor     import Meteor, MeteorManager
from Quad       import Quad
from HudQuad    import HudQuad
from PlayerInterface  import PlayerInterface
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
##############################################


class GameState(State):
  MAPPING = GameMapping
  def __init__(self, track='Track1', nPlayers=1):
    State.__init__(self)
    self.loaded = False
    
    self.laps   = 2 # TODO CONST
    self.stats  = {}
    self.gameOver = False

    
    self.load(track,nPlayers)
    
  def Activate(self):
    State.Activate(self)
    #if not self.loaded:
    #  game().PushState(LoadingState(self.load))
    print "Activate game state"

  def Deactivate(self):
    State.Deactivate(self)    
    # self.sound.isPaused = True
    # game().sound.sound.UpdateSoundFx(self.sound)
    
  def Pop(self):
    self.meteorManager.Release()
    del self.meteorManager
    
    for i in self.entities.values():
      self.Remove(i)
        
    self.vehicleList = []
    del self.track
    del self.player

    # print '*******'
    # import gc
    # print gc.collect()
    
    # print '\n\n'.join(map(repr,gc.get_referrers(self)))
    # print '*******'
    # print '\n\n'.join(gc.garbage)
    
    # if self in gc.garbage:
    #   print 'AAAAAAAAHHHHHHH'
    

  def AddAICar(self, name, orient, modelNum):
      ai    = self.Add(Vehicle(
        name,    
        self.track, 
        orient, 
        modelNum
      ))
      self.vehicleList.append(ai)
      AIBehavior(ai, self.track)
      self.Add(Shadow(ai))
    

  def load(self, track, nPlayers):
    # testing stuff
    # game().sound.PlaySound2D("jaguar.wav")
    print "GameState::load begin"
    scene = cpp.Scene()
    self.scene = scene
    
    # TODO
    # can we render a fake loading screen here until the real one works?
    
    self.track = self.Add(Track(track))
    self.vehicleList = []

    frame = self.track.GetFrame(-30.0)
    frametx = Matrix(frame.position, frame.up, frame.fw)
    
    forwardMat = Matrix(ORIGIN, -PI/2.0, X)
    
    self.player = self.Add(Vehicle("Player", self.track, 
      Matrix(Point3(0, 3, 0)) * frametx,
    ))
    PlayerBehavior(self.player)
    self.Add(Shadow(self.player))
    self.player.Backwards = False;
    self.vehicleList.append(self.player)
    
    self.AddAICar("AI1", Matrix(Point3(-15, 3, 0)) * frametx, 2)
    self.AddAICar("AI2", Matrix(Point3(+15, 3, 0)) * frametx, 5)
    self.AddAICar("AI3", Matrix(Point3(0, 3, -15)) * frametx, 2)
    #self.AddAICar("AI4", Matrix(Point3(-15, 3, -15)) * frametx, 5)
    #self.AddAICar("AI5", Matrix(Point3(+15, 3, -15)) * frametx, 2)
    #self.AddAICar("AI6", Matrix(Point3(+ 0, 3, -30)) * frametx, 5)
    #self.AddAICar("AI7", Matrix(Point3(-15, 3, -30)) * frametx, 2)
    #self.AddAICar("AI8", Matrix(Point3(+15, 3, -30)) * frametx, 5)

    startFrame = self.track.GetFrame(0.0)
    
    # TODO: this should load "StartLine.x" but it is not appearing properly
    finishLineTransform = Matrix(30, 1, 3) * Matrix(startFrame.position+startFrame.up, startFrame.up, startFrame.fw)
    self.Add(Model('Finish Line','FinishLine.x',None,finishLineTransform))

    self.skybox = SkyBox()
    
    self.interfaces = []

    viewports = self.SetupViewports(nPlayers)
      
    for viewport in viewports:
      pi = PlayerInterface(self, self.player, viewport)
      pi.AddRenderable(self.scene)
      pi.AddRenderable(self.skybox)
      self.interfaces.append(pi)

       
    self.meteorManager = MeteorManager(self)

    for i in range(CONSTS.NUM_METEORS):
      self.meteorManager.spawnRandom()
    
    self.lastMeteorTime = 0
    
    #self.sound = cpp.SoundFx();
    #self.sound.looping  = True
    #self.sound.is3D     = False
    #self.sound.isPaused = False
    #game().sound.sound.LoadSoundFx("Adventure.mp3", self.sound)
    
    game().time.Zero()
    self.loaded = True
  

  def SetupViewports(self, nPlayers):  
    ww = game().window.width
    wh = game().window.height
    
    wwh = ww/2
    whh = wh/2
    if nPlayers==1:
      return [(0,0,ww,wh)]
    elif nPlayers==2:
      return [
        (0,   0,  wwh,  wh),
        (wwh, 0,  wwh,  wh),
      ]
    elif nPlayers==4:
      return [
        (0,   0,    wwh, whh),
        (wwh, 0,    wwh, whh),
        (0,   whh,  wwh, whh),
        (wwh, whh,  wwh, whh),
      ]
    
  
  AIMED_METEOR_INTERVAL = 2.
    
  def Tick(self, time):
    
    # int SetOrientation3D(const Point3& listenerPos, const Vector3& listenerVel, const Vector3& atVector, const Vector3& upVector); //For 3D sound
    cam = self.interfaces[0].view.camera
    # TODO camera velocity
    game().sound.sound.SetOrientation3D(cam.GetPosition(), Point3(0,0,0), cam.GetLookAt(), cam.GetUp())
    
    _time.sleep(CONSTS.SLEEP_TIME)
    

    # TODO use a sort
    
    def TrackPosition(vehicle):
      return vehicle.trackpos
      
    self.vehicleList.sort(key = TrackPosition)
    
    for place,vehicle in enumerate(self.vehicleList):
      vehicle.place = place
      vehicle.lapRatio = vehicle.lapcount <= self.laps and vehicle.trackpos / self.track.dist % 1.0 or 1.0
    
    for interface in self.interfaces:
      interface.Tick(time)
      game().graphics.views.append(interface.view)
      game().graphics.views.append(interface.hud)

    
    # if not self.gameOver:
    #   self.lastMeteorTime += time.game_delta
    #   if self.lastMeteorTime > self.AIMED_METEOR_INTERVAL*time.RESOLUTION:
    #     self.lastMeteorTime = 0
    #     self.meteorManager.spawnTargeted(self.player)
    
    self.meteorManager.Tick(time)
    
    State.Tick(self, time)
    
  

      
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
    self.viewIndex[0] = (self.viewIndex[0]+1) % len(self.viewSets[0])
    
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
      
  def ObstacleAheadEvent(self, vehicleId, obstacleId):
    vehicle = self.entities[vehicleId]
    obstacle = self.entities[obstacleId]
    vehicle.obstacles.append(obstacle)
    
      

