from Core.Globals     import *
from Core.Menu        import *
from Core.Config      import Config
from MenuState        import MenuState
from MenuMapping      import *
import os
import os.path


class HighScoreState(MenuState):
  MAPPING = MainMenuMapping
  def __init__(self):
    MenuState.__init__(self)

    tracknames = {}
    laps = {}

    self.stats = []
    self.loaded = False
    
    self.menu = [
      ApplyMenuItem('Back', self.Menu_Back),
    ]

    try:
      if os.path.exists(Config.USER_STATS):
        for line in open(Config.USER_STATS, 'r').readlines():
          line = line.split('\t')
          stat = Struct(track=line[0], laps=int(line[1]), name=line[2], time=float(line[3]))
          self.stats.append(stat)
          tracknames[stat.track] = 1
          laps[stat.laps] = 1
        self.current = []
        self.curTrack = len(tracknames.keys())>0 and tracknames.keys()[0] or None
        self.curLaps = len(laps.keys()) and laps.keys()[0] or None
        self.update()    
        self.menu.append(SelectMenuItem('Track', self.Menu_Track, [(i,) for i in tracknames], 0))
        self.menu.append(SelectMenuItem('Laps', self.Menu_Laps, [(i and str(i) or 'Best',i) for i in laps.keys()], 0))
        self.loaded = True
        
    except:
      import traceback
      traceback.print_exc()
                  
    
  def update(self):
    self.current = [i for i in self.stats if i.track==self.curTrack and i.laps==self.curLaps]
    self.current.sort(key=lambda x:x.time)    
    
  def Tick(self, time):
    MenuState.Tick(self, time)
    
    y = self.menuTop + 200
    
    if self.loaded:
      header = self.curLaps and 'Total Time' or 'Best Lap'
      self.view.WriteString('Player', 'Sony Sketch EF', 28, 100, y)
      self.view.WriteString(header,   'Sony Sketch EF', 28, 500, y)
      y += 40
      
      for i, stat in enumerate(self.current[:20]):
        self.view.WriteString(str(i+1),           'Sony Sketch EF', 22, 60, y)
        self.view.WriteString(stat.name,          'Sony Sketch EF', 22, 100, y)
        self.view.WriteString('%.2f' % stat.time, 'Sony Sketch EF', 22, 500, y)
        y += 30
    else:
      self.view.WriteString('Could not load high score file - have you played yet?', 'Sony Sketch EF', 28, 100, y)
    
    
  def Menu_Back(self):
    game().PopState()
    
  def Menu_Track(self, value):
    self.curTrack = value[0]
    self.update()
    
  def Menu_Laps(self, value):
    self.curLaps = value[1]
    self.update()
    
    

    
    
    
    
    
