import threading
import random
import time as _time

from Core.Globals   import *
from Game.State     import State

from GameMapping    import *
from MenuState      import PauseMenuState
from GameEndState   import GameEndState
from GameSettings   import GameSettings

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
  
  def __init__(self, settings):
    State.__init__(self)
    self.loaded = False
    
    self.laps   = 2 # TODO CONST
    self.stats  = {}
    self.gameOver = False
    self.nPlayersRacing = settings.nPlayers
    self.freeAINames =  [
      "Arthur Dent", 
      "Ford Prefect", 
      "Zaphod Beeblebrox", 
      "Marvin", 
      "Trillian", 
      "Slartibartfast",
      "Philip J. Fry",
      "Bender Bending Rodriguez",
      "Turanga Leela"]

    
    self.load(settings)
    
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
    self.stats = {}
    del self.track

    # print '*******'
    # import gc
    # print gc.collect()
    
    # print '\n\n'.join(map(repr,gc.get_referrers(self)))
    # print '*******'
    # print '\n\n'.join(gc.garbage)
    
    # if self in gc.garbage:
    #   print 'AAAAAAAAHHHHHHH'
    
 
  def AddVehicle(self, player = None):
      if player: print vars(player)
      n = len(self.vehicleList)    
      x = (n % 3 - 1)*15
      z = ((n / 3))*-15
      
      vehicle    = self.Add(Vehicle(
        player and player.name or self.freeAINames.pop(),    
        self.track, 
        Matrix(Point3(x, 7, z)) * self.startOrientation, 
        player and player.textureId or random.choice(GameSettings.TEXTURE_IDS)
      ))
      vehicle.finishPlace = -1
      vehicle.isAI = player==None
      self.Add(Shadow(vehicle))
      self.vehicleList.append(vehicle)
      if player:
        PlayerBehavior(vehicle)
        vehicle.Backwards = False #???
      else:
        AIBehavior(vehicle, self.track)
      
      return vehicle                
      
  # def GetRandomTextureId(self):
  #   return len(self.freeTextureIds) > 0 and self.freeTextureIds.pop() or random.choice(self.TEXTURE_IDS)
    

  def load(self, settings):
    # testing stuff
    # game().sound.PlaySound2D("jaguar.wav")
    print "GameState::load begin"
    scene = cpp.Scene()
    self.scene = scene
    
    # TODO
    # can we render a fake loading screen here until the real one works?
    
    #self.freeTextureIds = settings.freeTextureIds
    #random.shuffle(self.freeTextureIds)
    #random.shuffle(self.AI_MODEL_NUMS)
    random.shuffle(self.freeAINames)
    
    
    self.track = self.Add(Track(settings.track))
    self.vehicleList = []

    frame = self.track.GetFrame(-30.0)
    self.startOrientation = Matrix(frame.position, frame.up, frame.fw)
    
    forwardMat = Matrix(ORIGIN, -PI/2.0, X)
    
    startFrame = self.track.GetFrame(0.0)
    
    # TODO: this should load "StartLine.x" but it is not appearing properly
    finishLineTransform = Matrix(30, 1, 3) * Matrix(startFrame.position+startFrame.up, startFrame.up, startFrame.fw)
    self.Add(Model('Finish Line','FinishLine.x',None,finishLineTransform))

    self.skybox = SkyBox()

    self.interfaces = []
    print "Number of players",len(settings.players)
    viewports = self.SetupViewports(len(settings.players))
    
    self.playerVehicles = [self.AddVehicle(player) for player in settings.players] 

    for i in range(settings.nAIs):
      self.AddVehicle(None)

    for i,vehicle in enumerate(self.playerVehicles):
      pi = PlayerInterface(self, vehicle, viewports[i])
      pi.AddRenderable(self.skybox)
      pi.AddRenderable(self.scene)
      self.interfaces.append(pi)


    self.SetupInputMapping(settings)

       
    self.meteorManager = MeteorManager(self)

    for i in range(CONSTS.NUM_METEORS):
      self.meteorManager.spawnRandom()
    
    self.lastMeteorTime = 0
    
    #self.sound = cpp.SoundFx();
    #self.sound.looping  = True
    #self.sound.is3D     = False
    #self.sound.isPaused = False
    #self.sound.volume = 64
    #game().sound.sound.LoadSoundFx("Adventure.mp3", self.sound)
    
    game().time.Zero()
    self.loaded = True
  

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
    elif nPlayers==4:
      return [
        (0,   0,  w2, h2),
        (w2,  0,  w2, h2),
        (0,   h2, w2, h2),
        (w2,  h2, w2, h2),
      ]
      
  def SetupInputMapping(self, settings):
    mappings = []
    
    for mapping in settings.debugMappings:
      mappings.append(mapping(None))
      
    for i,player in enumerate(settings.players):
      mappings.append(player.mapping(self.interfaces[i]))
      
    self.mapping = GameMapping(mappings)
    
  
  def Tick(self, time):
    
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
    self.stats.setdefault(vehicle, []).append(game().time.get_seconds())
    
    if lap == self.laps+1:
      print vehicle.name, " made it. he is an AI ",vehicle.isAI
      print self.nPlayersRacing," human players still racing"
      vehicle.finishPlace = vehicle.place
      if not vehicle.isAI:
        self.nPlayersRacing-=1
        if self.nPlayersRacing == 0:
          self.gameOver = True
          game().PushState(GameEndState(self.stats))
        
      vehicle.Brake(1)
    
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
    
      

