from Core.Globals     import *
from Core.Menu        import *
from Core.Config      import Config
from MenuState        import MenuState
from HudQuad          import HudQuad
from MenuMapping      import *
import os
import os.path
from Track import Track


class HighScoreState(MenuState):
  MAPPING = MainMenuMapping
  def __init__(self):
    MenuState.__init__(self)
    
    self.ui = HudQuad("UI Overlay", 'highscore-ui.png', 0, 0, 800, 600)
    self.view.Add(self.ui)

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
        self.curTrack = len(tracknames.keys())  and tracknames.keys()[0]  or None
        self.curLaps =  len(laps.keys())        and laps.keys()[0]
        
        trackoptions = []
        for track in tracknames:
          if track in Track.tracks:
            trackoptions.append((Track.tracks[track].NAME,track))
        
        self.menu.append(SelectMenuItem('Track', self.Menu_Track, trackoptions, 0, labelwidth=110))
        self.menu.append(SelectMenuItem('Laps', self.Menu_Laps, [(i and str(i) or 'Best',i) for i in laps.keys()], 0, labelwidth=110))
        self.loaded = True
        print self.curTrack, self.curLaps
        self.update()
        
        self.setBackground('Trackbg-%s.png' % self.curTrack)
        
    except:
      import traceback
      traceback.print_exc()
    
    
  def update(self):
    self.current = [i for i in self.stats if i.track==self.curTrack and i.laps==self.curLaps]
    self.current.sort(key=lambda x:x.time)  
    self.current = self.current[:10:]  
    
  def Tick(self, time):
    MenuState.Tick(self, time)
    
    y = self.menuTop + 200
    
    if self.loaded:
      header = self.curLaps and 'Total Time' or 'Best Lap'
      self.view.WriteString('Player', Config.FONT, 28, 120, y)
      self.view.WriteString(header,   Config.FONT, 28, 500, y)
      y += 40
      
      for i, stat in enumerate(self.current[:20]):
        self.view.WriteString(str(i+1),           Config.FONT, 22, 80, y)
        self.view.WriteString(stat.name,          Config.FONT, 22, 120, y)
        self.view.WriteString('%.2f' % stat.time, Config.FONT, 22, 520, y)
        y += 25
    else:
      self.view.WriteString('Could not load high score file - have you played yet?', Config.FONT, 28, 100, y)
    
    
  def Menu_Back(self):
    game().PopState()
    
  def Menu_Track(self, value):
    self.curTrack = value[1]
    self.update()
    self.setBackground('Trackbg-%s.png' % self.curTrack)        
    
  def Menu_Laps(self, value):
    self.curLaps = value[1]
    self.update()
    
    

    
    
    
    
    
