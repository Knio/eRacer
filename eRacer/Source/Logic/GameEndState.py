from Core.Globals import *

from Core.Config    import Config
from Game.State     import State
from GameEndMapping import GameEndMapping
from MenuState      import PauseMenuState
from Graphics.View  import View, HudView
from HudQuad        import HudQuad

class GameEndState(State):
  MAPPING = GameEndMapping
  def __init__(self, stats, parent):
    State.__init__(self)    
    self.parent = parent
    self.stats = stats
    self.view = HudView([self.scene])
    self.view.Add(HudQuad("TextBox", Config.UI_TEXTURE, 20,110,760,420, False))
    gameover = HudQuad("GameOverHeadline", "gameover_glow.png", 300,110,110,30, False)
    gameover.SetCenter(350,125)
    self.view.Add(gameover)
    
    
    
    # write stats
    with open(Config.USER_STATS, 'a') as f:
      for stat in self.calcstats():
        # i = [player, lap1, lap2, ..., total]
        if stat.player.isAI: continue
        f.write('%s\t%d\t%s\t%.4f\n' % (
          parent.track.name,
          parent.laps,
          stat.player.name,
          stat.total,
          )
        )
        
        best = min(stat.laps)
        f.write('%s\t%d\t%s\t%.4f\n' % (
          parent.track.name,
          0,
          stat.player.name,
          best,
          )
        )

  def calcstats(self):
    laps = self.parent.laps
    
    stats = []
    for v, s in self.stats.items():
      s = list(s)
      l = []
      
      for i in range(1,len(s)):
        l.append(s[i]-s[i-1])

      total = s[-1] - s[0]
      while len(l) < laps:
        l.append(99999)
        total = 99999
      
      stat = Struct(player=v, laps=l, total=total)

      stats.append(stat)
    
    stats.sort(key=lambda x:x.total)
    return stats
    
  def Tick(self, time):
    if not self.active:
      State.Tick(self, time)
      self.parent.Tick(time)
      return
    
    stats = self.calcstats()
    
    font = Config.FONT
    xd = 150
    yd = 30
    
    laps = ''.join('Lap %s     ' % i for i in range(1, len(stats[0].laps)+1))
    x = 100
    y = 200
    self.view.WriteString("Name", font, 28, x, y)
    x += xd
    for i in range(1, len(stats[0].laps)):
      self.view.WriteString("Lap %d" % i, font, 28, x, y)
      x += xd/2
    x += xd/2
    self.view.WriteString("Total", font, 28, x,y)
    
    y += yd
    x = 100
    for stat in stats:
      self.view.WriteString(stat.player.name, font, 24, x, y)
      x += xd
      for lap in stat.laps:
        s = "%6.2f" % lap
        if lap == 99999: s = '---'
        self.view.WriteString(s, font, 24, x, y)
        x += xd/2
      x += xd/2
      s = "%6.2f" % stat.total
      if stat.total == 99999: s = '---'
      self.view.WriteString(s, font, 24, x, y)
      x = 100
      y += yd
    
    State.Tick(self, time)
    self.parent.Tick(time)
    game().graphics.views.append(self.view)
    
  def LapEvent(self, vehicle, lap):
    self.parent.LapEvent(vehicle, lap)
  
  def PauseEvent(self):
    game().PushState(PauseMenuState())
    
  def CameraChangedEvent(self):
    self.parent.CameraChangedEvent()
