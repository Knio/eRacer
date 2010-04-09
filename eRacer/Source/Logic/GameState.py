import threading
import random
import time as _time

from Core.Globals   import *
from Game.State     import State

from GameMapping    import *
from MenuState      import PauseMenuState
from GameEndState   import GameEndState
from GameSettings   import GameSettings
from PlayerInterface  import PlayerInterface

# Entities
from Track      import Track
from Vehicle    import Vehicle
from Shadow     import Shadow
from Meteor     import Meteor, MeteorManager
from Quad       import Quad
from HudQuad    import HudQuad

# AI stuff
from AI.Behavior import PlayerBehavior, AIBehavior
from AI.Raceline import Raceline

# View stuff
from Graphics.View    import View
from Graphics.SkyBox  import SkyBox
    

    # TODO
    # can we render a fake loading screen here until the real one works?

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
  def __init__(self, settings):
    State.__init__(self)
    self.loaded = False
    self.load(settings)
 

  def load(self, settings):
    self.settings = settings
   
    self.laps   = self.settings.nLaps
    self.stats  = {}
 
    self.gameOver = False
 
    self.nPlayersRacing = self.settings.nPlayers
 
    self.freeAINames =  list(GameSettings.AI_NAMES)
    random.shuffle(self.freeAINames)
    
    track = Track(self.settings.track)
    self.vehicleList = []

    frame = track.GetFrame(-30.0)
    self.startOrientation = Matrix(frame.position, frame.up, frame.fw)
    
    forwardMat = Matrix(ORIGIN, -PI/2.0, X)
    
    startFrame = track.GetFrame(0.0)
    
    finishLineTransform = Matrix(40, 8, 1) * Matrix(startFrame.position+startFrame.up*0.1+startFrame.fw*-3, startFrame.fw, -startFrame.up)
    self.Add(Quad('FinishLine','FinishLine2.png',finishLineTransform))
    #self.Add(Model('Finish Line','FinishLine.x',None,finishLineTransform))

    for x in [200., 2200.]:
      for i in xrange(64):
        frame = track.GetFrame(x+10*i)
        tx = Matrix(3.0, 3.0, 6.0) * Matrix(frame.position, frame.up, frame.fw)
        self.Add(Model("Ring", 'Ring2.x', None, tx))

    self.track = self.Add(track)
      
    
    self.skybox = SkyBox()

    self.interfaces = []
    viewports = self.SetupViewports(self.settings.nPlayers)
    
    self.playerVehicles = [self.AddVehicle(player) for player in self.settings.players] 

    for i in range(self.settings.nAIs):
      self.AddVehicle(None)

    for i,vehicle in enumerate(self.playerVehicles):
      pi = PlayerInterface(self, vehicle, viewports[i])
      pi.AddRenderable(self.skybox)
      pi.AddRenderable(self.scene)
      self.interfaces.append(pi)

    self.SetupInputMapping()
       
    self.meteorManager = MeteorManager(self)

    for i in range(CONSTS.NUM_METEORS):
      self.meteorManager.spawnRandom()
    
    self.lastMeteorTime = 0
    
    # self.LoadMusic("Adventure.mp3")

    self.music.volume = 20
    self.LoadMusic("Adventure.mp3")
        
    self.boostbeams = []
    for i in xrange(16):
      beam = Model('StealBeam%d'%i, 'boostStealBeam.x', None, IDENTITY)
      beam.active = False
      self.boostbeams.append(self.Add(beam))
    
    game().time.Zero()
    self.loaded = True
  
  def BoostStealEvent(self, stealer, stealee, tx):
    for b in self.boostbeams:
      if b.active: continue
      b.active = True
      b.graphics.visible = True
      b.transform = tx
      return
      
  def AddVehicle(self, player = None):
    if player: print vars(player)
    n = len(self.vehicleList)    
    x = (n % 3 - 1)*15
    z = ((n / 3))*-15
    
    vehicle    = self.Add(Vehicle(
      player and player.name or self.freeAINames.pop(),    
      self.track, 
      Matrix(Point3(x, 7, z)) * self.startOrientation, 
      player and player.textureId or self.settings.RandomTextureId()
    ))
    vehicle.finishPlace = -1
    vehicle.lapBugCount = 0
    vehicle.isAI = player==None
    self.Add(Shadow(vehicle))
    self.vehicleList.append(vehicle)
    if player:
      PlayerBehavior(vehicle)
      vehicle.Backwards = False #???
    else:
      AIBehavior(vehicle, self.track)
    
    return vehicle                
      

  def SetupViewports(self, nPlayers):  
    w = game().graphics.width
    h = game().graphics.height
    
    w2 = w/2
    h2 = h/2
    if nPlayers==1:
      return [(0,0,w,h)]
    elif nPlayers==2:
      return [
        (0,   0,  w, h2),
        (0,   h2, w, h2),
      ]
    elif nPlayers>2:
      return [
        (0,   0,  w2, h2),
        (w2,  0,  w2, h2),
        (0,   h2, w2, h2),
        (w2,  h2, w2, h2),
      ]
      
  def SetupInputMapping(self):
    mappings = []
    
    for mapping in self.settings.debugMappings:
      mappings.append(mapping(None))
        
    for i,player in enumerate(self.settings.players):
      mappings.append(player.mapping(self.interfaces[i]))
      
    if self.settings.nPlayers==1 and self.settings.players[0].mapping != Keyboard1Mapping:
      mappings.append(Keyboard1Mapping(self.interfaces[0]))
      
    self.mapping = GameMapping(mappings)
    
  
  
  def Tick(self, time):
    
    for b in self.boostbeams:
      b.active = False
      b.graphics.visible = False
    
    # int SetOrientation3D(const Point3& listenerPos, const Vector3& listenerVel, const Vector3& atVector, const Vector3& upVector); //For 3D sound
    if len(self.interfaces) > 0:
      cam = self.interfaces[0].view.camera
      # TODO camera velocity
      game().sound.sound.SetOrientation3D(cam.GetPosition(), Point3(0,0,0), cam.GetLookAt(), cam.GetUp())
    
    _time.sleep(CONSTS.SLEEP_TIME)
    

    self.vehicleList.sort(key = lambda vehicle:vehicle.trackpos, reverse=True)
    
    for place,vehicle in enumerate(self.vehicleList):
      vehicle.place = place+1
      vehicle.lapRatio = vehicle.lapcount <= self.laps and vehicle.trackpos / self.track.dist % 1.0 or 1.0
    
    for interface in self.interfaces:
      interface.Tick(time)
      game().graphics.views.append(interface.view)
      game().graphics.views.append(interface.hud)

    
    if (not self.gameOver) and CONSTS.AIMED_METEOR_INTERVAL:
      self.lastMeteorTime += time.game_delta
      if self.lastMeteorTime > CONSTS.AIMED_METEOR_INTERVAL*time.RESOLUTION:
        self.lastMeteorTime = 0
        self.meteorManager.spawnTargeted(random.choice(self.vehicleList))
    
    self.meteorManager.Tick(time)
    
    State.Tick(self, time)
      
  def LapEvent(self, vehicle, lap):
    print "Lap Event %d" % (lap) 
    #len(self.stats[vehicle])
    if vehicle.lapBugCount < lap:
      self.stats.setdefault(vehicle, []).append(game().time.get_seconds())
      vehicle.lapBugCount+=1
    
    if lap == self.laps+1:
      vehicle.finishPlace = vehicle.place
      if not vehicle.isAI:
        self.nPlayersRacing-=1
        if self.nPlayersRacing == 0:
          self.gameOver = True
          game().PushState(GameEndState(self.stats))
        
      vehicle.Brake(1)
      vehicle.isShutoff = True
      
    
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
    
  def Release(self):
    self.loaded = False
    self.meteorManager.Release()
    self.PauseMusic()
    del self.meteorManager
    
    for i in self.entities.values():
      self.Remove(i)
    del self.track
    
    
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
    self.Release()
        
    self.vehicleList = []
    self.stats = {}
      

