from Core.Globals     import *
from Game.State       import State
from Graphics.View    import View, HudView
from HudQuad          import HudQuad
from Core.Config      import Config
from Mapping          import Mapping, E


class CreditsMapping(Mapping):
  def __init__(self):
    pass
    
  def KeyPressedEvent(self, key):
    if key   == KEY.ESCAPE:   return E.QuitEvent()
    elif key == KEY.RETURN: return E.MenuSelectEvent()
    
  def GamepadButtonPressedEvent(self, id, button):
    if button == cpp.BUTTON_A: return E.MenuSelectEvent()

class CreditsState(State):
  MAPPING = CreditsMapping
  SCROLLING_SPEED = 30 #pixels per second
  LEFT = 150
  
  def __init__(self):
    State.__init__(self)
    
    self.data = []
    for line in open(Config.CREDITS_FILE, 'r').readlines():
      if line.strip() == '':
        continue
      s = Struct()
      if line.startswith('h3. '):
        s.paddingTop = 40
        s.lineheight = 16
        s.fontsize = 24
        s.color = cpp.WHITE
        s.text = line[4::]
      else:
        s.paddingTop = 10
        s.lineheight = 20
        s.fontsize = 30
        s.color = cpp.GREEN
        s.text = line
        
        
      self.data.append(s)
      
      
    self.view = HudView([self.scene])
    self.top = 550

    self.logo = HudQuad("Logo","eRacerXLogoNegative.png", CreditsState.LEFT, self.top-150, 535, 212)
    self.view.Add(self.logo)
      
  def Tick(self, time):
    State.Tick(self, time)
 
    t = self.top

    self.logo.SetLeftTop(CreditsState.LEFT, t-150)

    self.view.WriteString("Credits", Config.FONT, 60, CreditsState.LEFT, t, cpp.WHITE)
    t += 60

    
    for i,line in enumerate(self.data):
      t += line.paddingTop + line.lineheight
      
      if t<-50:
        continue
      
      self.view.WriteString(line.text, Config.FONT, line.fontsize, CreditsState.LEFT, t+line.lineheight, line.color)
      
      if t>600:
        break
    
    self.top -= (float(time.wall_delta)/time.RESOLUTION)*CreditsState.SCROLLING_SPEED
    
    game().graphics.views.append(self.view)
    
  def MenuSelectEvent(self):
    game().PopState()

  def KeyPressedEvent(self, key):
    if key in [KEY.DOWN, KEY.SPACE]:
      self.top -= 300
    if key in [KEY.UP]:
      self.top += 300
      