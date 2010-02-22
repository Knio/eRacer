from Core.Globals   import *
from Game.State     import State
  
from Camera         import CirclingCamera, OrthographicCamera
from Graphics.Sprite         import Sprite
from MenuMapping    import MainMenuMapping, PauseMenuMapping
from Graphics.View  import View

class MenuState(State):
  MENU = []
  def __init__(self):
    State.__init__(self)
    self.selected = 0
    
  def Tick(self, time):
    State.Tick(self, time)
    
    self.view = View(OrthographicCamera(8,6))
    #width and height should not be hardcoded!
    game().graphics.views.append(self.view)
    
    game().logic.Add(Sprite(self.view,Point3(10,10,2)))
    
    
    
    y = 200
    for i,m in enumerate(self.MENU):
      name = m[0]
      game().graphics.graphics.WriteString(
        name, "Verdana", 32, Point3(100,y,0), i==self.selected and RED or WHITE
      ) 
      y += 50
      
  def MenuUpEvent(self):
    self.selected = (self.selected-1) % len(self.MENU)
  
  def MenuDownEvent(self):
    self.selected = (self.selected+1) % len(self.MENU)

  def MenuSelectEvent(self):
    name = self.MENU[self.selected][0]
    getattr(self, 'Menu_%s' % name.replace(' ','_'))()
    
  def Menu_Exit(self):
    game().event.QuitEvent()
  
    
class MainMenuState(MenuState):
  MAPPING = MainMenuMapping
  MENU = [
    ('New Game',),
    ('Exit',)
  ]
  
  def __init__(self):
    MenuState.__init__(self)
    
  def Menu_New_Game(self):
    game().PushState(GameState())
        
  def Tick(self, time):
    game().graphics.graphics.WriteString(
      "eRacerX", 
      "Verdana", 128, Point3(180,60,0)
    )
    
    
    p = Point3(500,350,0)
    for i in ['Don Ha', 'John Stuart', 'Michael Blackadar', 'Tom Flanagan', 'Ole Rehmsen']:
      game().graphics.graphics.WriteString(
        i, "Verdana", 28, p
      )
      p = p + Point3(0, 30, 0)
    
    MenuState.Tick(self, time)    
    

class PauseMenuState(MenuState):
  MAPPING = PauseMenuMapping
  MENU = [
    ('Continue',),
    ('Exit',) 
  ]
  
  def __init__(self):
    MenuState.__init__(self)
    
  def Activate(self):
    game().simspeed = 0.
  
  def Deactivate(self):
    game().simspeed = 1.
    
  def UnPauseEvent(self):
    game().PopState()
    
  def Menu_Continue(self):
    game().PopState()
    
  def Tick(self, time):
    game().graphics.graphics.WriteString(
      "PAUSED",
      "Verdana", 40, Point3(300,100,0)
    )
    MenuState.Tick(self, time)
    self.parent.Tick(time)

from GameState  import GameState
