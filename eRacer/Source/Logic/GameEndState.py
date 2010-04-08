from Core.Globals import *

from Game.State     import State
from GameEndMapping import GameEndMapping
from MenuState      import PauseMenuState
from Graphics.View  import View, HudView
from HudQuad        import HudQuad

class GameEndState(State):
  MAPPING = GameEndMapping
  def __init__(self, stats):
    State.__init__(self)    
    self.stats = stats
    self.view = HudView([self.scene])
    textbox = HudQuad("TextBox", "futureui1.png", 50,107,700,400, True)
    self.view.Add(textbox)
    
  def Tick(self, time):
    if not self.active:
      State.Tick(self, time)
      self.parent.Tick(time)
      return
    
    self.view.WriteString(
      "GAME OVER",
      "Sony Sketch EF", 40, Point3(300,100,0), Vector3(1,0.5,0.25)
    )

    le = 0
    for v, s in self.stats.items():
      le = max(le, len(s))

    stats = []
    for v, s in self.stats.items():
      s = list(s)
      l = []
      l.append(v)
      
      for i in range(1,len(s)):
        l.append(s[i]-s[i-1])

      while len(l) < le:
        l.append(99999)
        s[-1] = 99999
      s = [0.] + s
      
      l.append(s[-1] - s[0])
      stats.append(l)
    
    stats.sort(key=lambda x:x[-1])
   
    font = "Sony Sketch EF"
    xd = 150
    yd = 30
    
    laps = ''.join('Lap %s     ' % i for i in range(1, len(stats[0])-1))
    x = 100
    y = 200
    self.view.WriteString("Name", font, 28, Point3(x,y,0))
    x += xd
    for i in range(1, len(stats[0])-1):
      self.view.WriteString("Lap %d" % i, font, 28, Point3(x,y,0))
      x += xd/2
    x += xd/2
    self.view.WriteString("Total", font, 28, Point3(x,y,0))
    
    y += yd
    x = 100
    for l in stats:
      self.view.WriteString(l[0].name, font, 24, Point3(x,y,0))
      x += xd
      for i in range(1, len(l)-1):
        s = "%6.2f" % l[i]
        if l[i] == 99999: s = '---'
        self.view.WriteString(s, font, 24, Point3(x,y,0))
        x += xd/2
      x += xd/2
      s = "%6.2f" % l[-1]
      if l[-1] == 99999: s = '---'
      self.view.WriteString(s, font, 24, Point3(x,y,0))
      x = 100
      y += yd
    
    
    game().graphics.views.append(self.view)
    State.Tick(self, time)
    self.parent.Tick(time)
    
  def LapEvent(self, vehicle, lap):
    self.parent.LapEvent(vehicle, lap)
  
  def PauseEvent(self):
    game().PushState(PauseMenuState())
    
  def CameraChangedEvent(self):
    self.parent.CameraChangedEvent()
