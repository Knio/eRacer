from Core.Globals     import *
from Core.Config      import Config
from Starfield        import Starfield
from Camera           import ChasingCamera, FirstPersonCamera, CarCamera, OrthographicCamera, CirclingCamera
from HudQuad          import HudQuad
from Graphics.View    import View, HudView
from GameEndState     import GameEndState

class PlayerInterface(object):
  def __init__(self, state, vehicle, viewport):
    self.state    = state # TODO try to get rid of this
    self.vehicle   = vehicle
    self.viewport = viewport
    self.views    = []
    self.viewIndex = 0
    
    self.icons = {}
    
    cam = state.Add(ChasingCamera(vehicle))
    self.views.append(View(cam, viewport=self.viewport))
    
    cam = state.Add(CarCamera(vehicle))
    self.views.append(View(cam, viewport=self.viewport))

    circCam = state.Add(CirclingCamera(vehicle))
    self.views.append(View(circCam, viewport = self.viewport))

    debugCam = state.Add(FirstPersonCamera())
    self.views.append(View(debugCam, viewport=self.viewport))
    
    self.hud      = HudView(viewport=self.viewport)
    self.hud.name = 'PI HudView'
    self.boostFuelMaxHeight = 291
    self.boostFuelTop = 200
    self.boostFuelBottom = self.boostFuelTop+self.boostFuelMaxHeight
    self.boostBar = self.AddHud(HudQuad("BoostBarFuel", "BoostBarFuel.png", 758, self.boostFuelTop, 29, self.boostFuelMaxHeight,True))
    self.AddHud(HudQuad("BoostBarFrame", "BoostBarFrame.png", 750, self.boostFuelTop-47, 44, 383,True))
    self.distanceBar = self.AddHud(HudQuad("DistanceBar", "CheckerBar.jpg", 150, 35, 500, 8, True))
    
    for vehicle in state.vehicleList:
      if vehicle == self.vehicle: continue
      self.icons[vehicle.name] = self.AddHud(HudQuad(vehicle.name+"Icon", "redmarker.png", 150-8, 50-12, 16, 16))
    self.icons[self.vehicle.name] = self.AddHud(HudQuad(self.vehicle.name+"Icon", "bluemarker.png", 150-8, 50-12, 16, 16))


    self.starfields = []
    self.starlen = 2
    for view in self.views:
      self.starfields.append(view.AddRenderable(state.Add(Starfield(1024, 1000.0, view.camera))))
      self.starfields.append(view.AddRenderable(state.Add(Starfield(1024, 100.0,  view.camera))))
      self.starfields.append(view.AddRenderable(state.Add(Starfield( 256, 20.0,   view.camera))))
    
  def get_view(self):
    return self.views[self.viewIndex]
  view = property(get_view)
  
  def AddRenderable(self, obj):
    for i in self.views:
      i.AddRenderable(obj)
      
  def AddHud(self,entity):
    self.state.Add(entity,False)
    self.hud.AddRenderable(entity)    
    return entity
    
  def Tick(self, time):
    delta = float(time.game_delta) / time.RESOLUTION
    
    if self.vehicle.boosting: self.starlen += delta * 15
    else:                     self.starlen -= delta * 15
    
    self.starlen = clamp(self.starlen,2,8)
    for i in self.starfields:
      i.length = int(self.starlen)

    countdown = math.ceil(self.state.countdown)
    if countdown == 3:
      self.hud.WriteString( "%d" % (countdown), Config.FONT, 150, 330, 200, Vector3(1,0,0))
    if countdown == 2:
      self.hud.WriteString( "%d" % (countdown), Config.FONT, 150, 330, 200, Vector3(1,1,0))
    if countdown == 1:
      self.hud.WriteString( "%d" % (countdown), Config.FONT, 150, 330, 200, Vector3(1,1,0))
    if countdown <= 0 and countdown >= -2:
      self.hud.WriteString( "GO!", Config.FONT, 150, 300, 200, Vector3(0,1,0))

    t = time.seconds*5

    #TEXT BOX HACK
    #self.textBox.SetSize( 0, 0)
    #self.boostBar.SetSize( 700, 400)
    
    #Track Place HUD
    place = self.vehicle.finishPlace < 0 and self.vehicle.place or self.vehicle.finishPlace
    self.hud.WriteString(ordinal(place), Config.FONT, 60, 20, 5)
    if not self.vehicle.finishPlace < 0 and self.state.active:
        self.hud.WriteString(ordinal(self.vehicle.finishPlace), Config.FONT, 80, 330, 350, Vector3(math.cos(t),math.sin(t),math.sin(t)))
    
  
    for vehicle in self.state.vehicleList:
      self.icons[vehicle.name].SetLeftTop(150-8 + 500 * vehicle.lapRatio, 35-12)

    #Energy Bar HUD 750, 200, 35, 350
    boostPercent = self.vehicle.boostFuel/5.0
    self.boostBar.graphics.SetTextureCoordinates(0,1-boostPercent,  1,1-boostPercent, 1,1, 0,1 );
    height = boostPercent * self.boostFuelMaxHeight
    self.boostBar.SetSize( 29, height)
    self.boostBar.SetLeftTop( 758, self.boostFuelBottom-height );  

    #Backwards HUD
    playerfacing = mul0(self.vehicle.transform, Z)
    playertrackfacing = self.vehicle.frame.fw
    playerdirection = dot(playerfacing, playertrackfacing)
    if self.vehicle.Backwards == False and self.vehicle.trackpos < self.vehicle.lasttrackpos and playerdirection < 0:
       self.vehicle.Backwards = True
    if self.vehicle.Backwards == True and self.vehicle.trackpos > self.vehicle.lasttrackpos and playerdirection > 0:
       self.vehicle.Backwards = False
    if self.vehicle.Backwards == True:
       self.hud.WriteString( "WRONG WAY", Config.FONT, 50, 300, 200)

    #Lap counter
    if self.vehicle.lapcount or True: # ???
      playerLaps = min(self.vehicle.lapcount, self.state.laps)
      playerLaps = max(1, playerLaps);
      
      self.hud.WriteString("%d" % (playerLaps), Config.FONT,96, 650, 0)
      self.hud.WriteString("/", Config.FONT, 80, 690, 20)
      self.hud.WriteString("%d" % (self.state.laps), Config.FONT, 80, 720, 30)
    
      l = list(self.state.stats.get(self.vehicle,[0.]))
      l.append(game().time.get_seconds())
      
      y = 100
      for i,t in enumerate(l):
        #if not i or i>self.state.laps or self.vehicle.lapcount == 0: continue
        #if not i or self.vehicle.lapcount == 0: continue
        if i>self.state.laps: continue
        if self.vehicle.lapBugCount > 0 and not i == 0:
          self.hud.WriteString("Lap %d:" % i, Config.FONT, 24, 650, y)
          self.hud.WriteString("%05.2f"   % (t-l[i-1]), Config.FONT, 24, 720, y)
          y += 15    
  
    #Personal Endgamestuff
    if self.vehicle.lapcount > self.state.laps:
      self.viewIndex = 2

  def CameraChangedEvent(self):
    #don't use last camera, it's the debug one
    self.viewIndex = (self.viewIndex+1) % (len(self.views) - 2)    

  def DebugCameraToggle(self):
    if self.viewIndex == len(self.views) - 1:
      #go back to standard camera
      self.viewIndex = 0
    else:
      #turn on debug camera
      self.viewIndex = len(self.views) - 1
    
  
    
  
