from Core.Globals import *

from Game.State import State

class GameEndState(State):
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

    stats = []
    for v, s in self.stats.items():
      l = []
      l.append(v)
      s = [0.] + s
      for i in range(1,len(s)):
        l.append(s[i]-s[i-1])
      l.append(s[-1])
      stats.append(l)
    
    print stats
    stats.sort(key=lambda x:x[-1])
    print stats
    
    
    xd = 150
    yd = 30
    
    laps = ''.join('Lap %s     ' % i for i in range(1, len(stats[0])-1))
    x = 100
    y = 200
    game().graphics.graphics.WriteString("Name", "Verdana", 24, Point3(x,y,0))
    x += xd
    for i in range(1, len(stats[0])-1):
      game().graphics.graphics.WriteString("Lap %d" % i, "Verdana", 24, Point3(x,y,0))
      x += xd
    game().graphics.graphics.WriteString("Total", "Verdana", 24, Point3(x,y,0))
    
    y += yd
    x = 100
    for l in stats:
      game().graphics.graphics.WriteString(l[0].name, "Verdana", 24, Point3(x,y,0))
      x += xd
      for i in range(1, len(l)-1):
        game().graphics.graphics.WriteString("%6.2f" % l[i], "Verdana", 24, Point3(x,y,0))
        x += xd
      game().graphics.graphics.WriteString("%6.2f" % l[-1], "Verdana", 24, Point3(x,y,0))
      x = 100
      y += yd
    
    State.Tick(self, time)
    self.parent.Tick(time)
    
    