
from Core.Globals     import *
from Core.Menu        import *
from Core.Config      import Config
from MenuState        import MenuState
from MenuMapping      import *

class HighScoreState(MenuState):
  MAPPING = MainMenuMapping
  def __init__(self):
    MenuState.__init__(self)

    tracknames = {}
    laps = {}

    self.stats = []
    try:
      for line in open(Config.USER_STATS, 'r').readlines():
        line = line.split('\t')
        stat = Struct(track=line[0], laps=int(line[1]), name=line[2], time=float(line[3]))
        self.stats.append(stat)
        tracknames[stat.track] = 1
        laps[stat.laps] = 1
        
    except:
      import traceback
      traceback.print_exc()
      
    
    self.current = []
    self.curTrack = tracknames.keys()[0]
    self.curLaps = laps.keys()[0]
    self.update()    
        
    self.menu = [
      ApplyMenuItem('Back', self.Menu_Back),
      SelectMenuItem('Track', self.Menu_Track, [(i,) for i in tracknames], 0),
      SelectMenuItem('Laps', self.Menu_Laps, [(i and str(i) or 'Best',i) for i in laps.keys()], 0),
    ]
    
    
  def update(self):
    self.current = [i for i in self.stats if i.track==self.curTrack and i.laps==self.curLaps]
    self.current.sort(key=lambda x:x.time)    
    
  def Tick(self, time):
    
    # self.view.WriteString(self.curTrack, "Verdana")
    MenuState.Tick(self, time)
  
    
    y = self.menuTop + 200
    
    header = self.curLaps and 'Total Time' or 'Best Lap'
    self.view.WriteString('Player', 'Sony Sketch EF', 28, Point3(100, y, 0))
    self.view.WriteString(header,   'Sony Sketch EF', 28, Point3(500, y, 0))
    y += 40
    
    for i, stat in enumerate(self.current[:20]):
      self.view.WriteString(str(i+1),           'Sony Sketch EF', 22, Point3(60, y, 0))
      self.view.WriteString(stat.name,          'Sony Sketch EF', 22, Point3(100, y, 0))
      self.view.WriteString('%.2f' % stat.time, 'Sony Sketch EF', 22, Point3(500, y, 0))
      y += 30
    
    
  def Menu_Back(self):
    game().PopState()
    
  def Menu_Track(self, value):
    self.curTrack = value[0]
    self.update()
    
  def Menu_Laps(self, value):
    self.curLaps = value[1]
    self.update()
    
    

    
    
    
    
    
