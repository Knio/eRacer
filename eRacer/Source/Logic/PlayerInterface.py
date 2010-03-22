from Core.Globals     import *
from Starfield        import Starfield
from Camera           import ChasingCamera, FirstPersonCamera, CarCamera, OrthographicCamera
from HudQuad          import HudQuad
from Graphics.View    import View

class PlayerInterface(object):
  def __init__(self, state, player, viewport):
    self.state    = state # TODO try to get rid of this
    self.player   = player
    self.viewport = viewport
    self.views    = []
    self.viewIndex = 0
    
    self.icons = {}
    
    
    cam = state.Add(ChasingCamera(player))
    self.views.append(View(cam, viewport=self.viewport))
    
    cam = state.Add(FirstPersonCamera())
    self.views.append(View(cam, viewport=self.viewport))
    
    cam = state.Add(CarCamera(player))
    self.views.append(View(cam, viewport=self.viewport))
    
    self.hud      = View(OrthographicCamera(game().graphics.width, game().graphics.height), viewport=self.viewport)
    self.boostBar = self.AddHud(HudQuad("BoostBar", "FinishLine.png", 750, 200, 35, 350))
    self.distanceBar = self.AddHud(HudQuad("DistanceBar", "CheckerBar.jpg", 150, 50, 500, 8))
    for vehicle in state.vehicleList:
      self.icons[vehicle.name] = self.AddHud(HudQuad(vehicle.name+"Icon", self.player==vehicle and "bluemarker.png" or "redmarker.png", 150-8, 50-12, 16, 16))

    for view in self.views:
      view.AddRenderable(state.Add(Starfield(1024, 1000.0, view.camera)))
      view.AddRenderable(state.Add(Starfield(4096, 100.0,  view.camera)))
      view.AddRenderable(state.Add(Starfield( 512, 20.0,   view.camera)))
    
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
    #Track Place HUD
    game().graphics.graphics.WriteString(self.ordinal(self.player.place), "Verdana", 60, Point3(20, 20,0))
    
  
    for vehicle in self.state.vehicleList:
      self.icons[vehicle.name].SetLeftTop(150-8 + 500 * vehicle.lapRatio, 50-12)

    #Energy Bar HUD 750, 200, 35, 350
    boostPercent = self.player.boostFuel/5.0
    self.boostBar.graphics.SetTextureCoordinates(0,1-boostPercent,  1,1-boostPercent, 1,1, 0,1 );
    height = boostPercent * 350
    self.boostBar.SetSize( 35, height)
    self.boostBar.SetLeftTop( 750, 550-height );  

    #Backwards HUD
    playerfacing = mul0(self.player.transform, Z)
    playertrackfacing = self.player.frame.fw
    playerdirection = dot(playerfacing, playertrackfacing)
    if self.player.Backwards == False and self.player.trackpos < self.player.lasttrackpos and playerdirection < 0:
       self.player.Backwards = True
    if self.player.Backwards == True and self.player.trackpos > self.player.lasttrackpos and playerdirection > 0:
       self.player.Backwards = False
    if self.player.Backwards == True:
       game().graphics.graphics.WriteString( "WRONG WAY", "Verdana", 50, Point3(300,200,0))


                                  
    # playerLaps = max(1, min(self.player.lapcount, self.state.laps))
    # game().graphics.graphics.WriteString("%d" % (playerLaps), "Sony Sketch EF",96, Point3(650, 0, 0))
    # game().graphics.graphics.WriteString("/", "Sony Sketch EF", 80, Point3(690, 20, 0))
    # game().graphics.graphics.WriteString("%d" % (self.laps), "Sony Sketch EF", 80, Point3(720, 30, 0))

    # if self.player.lapcount:
    #   l = list(self.stats.get(self.player,[0.]))
    #   l.append(game().time.get_seconds())


  

                                  
    if self.player.lapcount:
      playerLaps = min(self.player.lapcount, self.state.laps)
      
      game().graphics.graphics.WriteString("%d" % (playerLaps), "Sony Sketch EF",96, Point3(650, 0, 0))
      game().graphics.graphics.WriteString("/", "Sony Sketch EF", 80, Point3(690, 20, 0))
      game().graphics.graphics.WriteString("%d" % (self.state.laps), "Sony Sketch EF", 80, Point3(720, 30, 0))
    
      l = list(self.state.stats.get(self.player,[0.]))
      l.append(game().time.get_seconds())
      
      y = 100
      for i,t in enumerate(l):
        if not i or i>self.state.laps: continue
        game().graphics.graphics.WriteString("Lap %d:" % i, "Sony Sketch EF", 24, Point3(650, y, 0))
        game().graphics.graphics.WriteString("%05.2f"   % (t-l[i-1]), "Sony Sketch EF", 24, Point3(720, y, 0))
        y += 15    

  def CameraChangedEvent(self):
    self.viewIndex = (self.viewIndex+1) % len(self.views)    
    
  
    
  