from Core.Globals     import *
from Starfield        import Starfield
from Camera           import ChasingCamera, FirstPersonCamera, CarCamera, OrthographicCamera, CirclingCamera
from HudQuad          import HudQuad
from Graphics.View    import View

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
    
    debugCam = state.Add(FirstPersonCamera())
    self.views.append(View(debugCam, viewport=self.viewport))

    circCam = state.Add(CirclingCamera(vehicle))
    self.views.append(View(circCam, viewport = self.viewport))
    
    self.hud      = View(OrthographicCamera(game().graphics.width, game().graphics.height), viewport=self.viewport)
    self.boostBar = self.AddHud(HudQuad("BoostBar", "FinishLine.png", 750, 200, 35, 350))
    self.distanceBar = self.AddHud(HudQuad("DistanceBar", "CheckerBar.jpg", 150, 35, 500, 8))
    
    for vehicle in state.vehicleList:
      if not vehicle == self.vehicle:
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
    self.hud.AddRenderable(entity.graphics)    
    return entity
  
  def ordinal(self, num):
    if   num == 1: return "1st"
    elif num == 2: return "2nd"
    elif num == 3: return "3rd"
    else:          return "%1dth" % num
    
    
  def Tick(self, time):
    delta = float(time.game_delta) / time.RESOLUTION
    
    if self.vehicle.boosting: self.starlen += delta * 15
    else:                     self.starlen -= delta * 15
    
    self.starlen = max(min(self.starlen, 3), 2)
    for i in self.starfields:
      i.length = int(self.starlen)
    
    #Track Place HUD
    if self.vehicle.finishPlace < 0: 
      self.hud.WriteString(self.ordinal(self.vehicle.place), "Sony Sketch EF", 60, Point3(20, 5,0))
    else:
      self.hud.WriteString(self.ordinal(self.vehicle.finishPlace), "Sony Sketch EF", 60, Point3(20, 5,0))
    
  
    for vehicle in self.state.vehicleList:
      self.icons[vehicle.name].SetLeftTop(150-8 + 500 * vehicle.lapRatio, 35-12)

    #Energy Bar HUD 750, 200, 35, 350
    boostPercent = self.vehicle.boostFuel/5.0
    self.boostBar.graphics.SetTextureCoordinates(0,1-boostPercent,  1,1-boostPercent, 1,1, 0,1 );
    height = boostPercent * 350
    self.boostBar.SetSize( 35, height)
    self.boostBar.SetLeftTop( 750, 550-height );  

    #Backwards HUD
    playerfacing = mul0(self.vehicle.transform, Z)
    playertrackfacing = self.vehicle.frame.fw
    playerdirection = dot(playerfacing, playertrackfacing)
    if self.vehicle.Backwards == False and self.vehicle.trackpos < self.vehicle.lasttrackpos and playerdirection < 0:
       self.vehicle.Backwards = True
    if self.vehicle.Backwards == True and self.vehicle.trackpos > self.vehicle.lasttrackpos and playerdirection > 0:
       self.vehicle.Backwards = False
    if self.vehicle.Backwards == True:
       self.hud.WriteString( "WRONG WAY", "Sony Sketch EF", 50, Point3(300,200,0))

    #Lap counter
    if self.vehicle.lapcount or True: # ???
      playerLaps = min(self.vehicle.lapcount, self.state.laps)
      playerLaps = max(1, playerLaps);
      
      self.hud.WriteString("%d" % (playerLaps), "Sony Sketch EF",96, Point3(650, 0, 0))
      self.hud.WriteString("/", "Sony Sketch EF", 80, Point3(690, 20, 0))
      self.hud.WriteString("%d" % (self.state.laps), "Sony Sketch EF", 80, Point3(720, 30, 0))
    
      l = list(self.state.stats.get(self.vehicle,[0.]))
      l.append(game().time.get_seconds())
      
      y = 100
      for i,t in enumerate(l):
        #if not i or i>self.state.laps or self.vehicle.lapcount == 0: continue
        #if not i or self.vehicle.lapcount == 0: continue
        if i>self.state.laps: continue
        if self.vehicle.lapBugCount > 0 and not i == 0:
          self.hud.WriteString("Lap %d:" % i, "Sony Sketch EF", 24, Point3(650, y, 0))
          self.hud.WriteString("%05.2f"   % (t-l[i-1]), "Sony Sketch EF", 24, Point3(720, y, 0))
          y += 15    
  
    #Personal Endgamestuff
    if self.vehicle.lapcount > self.state.laps:
      self.viewIndex = 3

  def CameraChangedEvent(self):
    #don't use last camera, it's the debug one
    self.viewIndex = (self.viewIndex+1) % (len(self.views) - 2)    

  def SpinCameraEvent(self):
    self.viewIndex = 3
    
  def DebugCameraToggle(self):
    if self.viewIndex == len(self.views) - 1:
      #go back to standard camera
      self.viewIndex = 0
    else:
      #turn on debug camera
      self.viewIndex = len(self.views) - 1
    
  
    
  
