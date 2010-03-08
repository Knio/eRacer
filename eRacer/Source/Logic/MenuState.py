from Core.Globals   import *
from Game.State     import State
  
from Camera         import Camera, CirclingCamera, OrthographicCamera
from Graphics.Quad  import Quad
from MenuMapping    import MainMenuMapping, PauseMenuMapping
from Graphics.View  import View


from Box import Box


class MenuState(State):
  MENU = []
  def __init__(self):
    State.__init__(self)
    self.selected = 0
    
    #width and height should not be hardcoded!
    
    camera = OrthographicCamera(800,600)
    game().logic.Add(camera)

    self.view = View(camera)
        
    
  def Tick(self, time):
    State.Tick(self, time)
    game().graphics.views.append(self.view)
    
    
    y = 240
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

    logo = Quad(self.view,"eracerx_logo_negative.png")
    game().logic.Add(logo)

    logo.transform = Matrix(Point3(400,450,0), 0,0,0, 600,235,1)
    #logo.scale(600,235,1)
    #logo.set_translation(Point3(400,450,0))
    
    self.sound = eRacer.SoundFx();
    self.sound.looping = True
    self.sound.is3D = False
    self.sound.isPaused = False
    game().sound.sound.LoadSoundFx("Resources/Sounds/Terran5.ogg", self.sound)
    
  def Deactivate(self):
    MenuState.Deactivate(self)
    self.sound.isPaused = True
    game().sound.sound.UpdateSoundFx(self.sound)
    
    
  def Menu_New_Game(self):
    game().PushState(GameState())
        
  def Tick(self, time):
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
