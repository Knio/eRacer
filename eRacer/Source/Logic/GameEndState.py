from Core.Globals import *

from Game.State     import State
from GameEndMapping import GameEndMapping
from MenuState      import PauseMenuState

class GameEndState(State):
  MAPPING = GameEndMapping
  def __init__(self, stats):
    State.__init__(self)    
    self.stats = stats
    
  def Tick(self, time):
    game().graphics.graphics.WriteString(
      "GAME OVER",
      "Verdana", 40, Point3(300,100,0)
    )
    
    game().graphics.graphics.WriteString(
      "",
      "Verdana", 40, Point3(300,100,0)
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
      
      l.append(s[-1])
      stats.append(l)
    
    stats.sort(key=lambda x:x[-1])
   
    
    xd = 150
    yd = 30
    
    laps = ''.join('Lap %s     ' % i for i in range(1, len(stats[0])-1))
    x = 100
    y = 200
    game().graphics.graphics.WriteString("Name", "Verdana", 28, Point3(x,y,0))
    x += xd
    for i in range(1, len(stats[0])-1):
      game().graphics.graphics.WriteString("Lap %d" % i, "Verdana", 28, Point3(x,y,0))
      x += xd/2
    x += xd/2
    game().graphics.graphics.WriteString("Total", "Verdana", 28, Point3(x,y,0))
    
    y += yd
    x = 100
    for l in stats:
      game().graphics.graphics.WriteString(l[0].name, "Verdana", 24, Point3(x,y,0))
      x += xd
      for i in range(1, len(l)-1):
        s = "%6.2f" % l[i]
        if l[i] == 99999: s = '---'
        game().graphics.graphics.WriteString(s, "Verdana", 24, Point3(x,y,0))
        x += xd/2
      x += xd/2
      s = "%6.2f" % l[-1]
      if l[-1] == 99999: s = '---'
      game().graphics.graphics.WriteString(s, "Verdana", 24, Point3(x,y,0))
      x = 100
      y += yd
    
    State.Tick(self, time)
    self.parent.Tick(time)
    
  def LapEvent(self, vehicle, lap):
    self.parent.LapEvent(vehicle, lap)
  
  def PauseEvent(self):
    game().PushState(PauseMenuState())
    
  def CameraChangedEvent(self):
    self.parent.CameraChangedEvent()