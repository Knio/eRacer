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
    
    if not self.active:
      return
      
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
    logo.scale(600,235,1)
    logo.set_translation(Point3(400,450,0))
    game().logic.Add(logo)
    
    self.sound = eRacer.SoundFx();
    self.sound.looping = True
    self.sound.is3D = False
    self.sound.isPaused = False
    game().sound.sound.LoadSoundFx("Resources/Sounds/Terran5.ogg", self.sound)
        
  def Pause(self):
    self.sound.isPaused = True
    game().sound.sound.UpdateSoundFx(self.sound)
    
  def Menu_New_Game(self):
    # game().PushState(GameState())
    game().PushState(GameSelectState(self.view))
    
  def Tick(self, time):
    p = Point3(500,350,0)
    for i in ['Don Ha', 'John Stuart', 'Michael Blackadar', 'Tom Flanagan', 'Ole Rehmsen']:
      game().graphics.graphics.WriteString(
        i, "Verdana", 28, p
      )
      p = p + Point3(0, 30, 0)
    
    MenuState.Tick(self, time)
    
class GameSelectState(MainMenuState):
  MENU = [
    ('Track 1',),
    ('Track 2',),
    ('Back',),
  ]
  
  def __init__(self, view):
    MenuState.__init__(self)
    
    self._view = self.view
    
    # image1 = Quad(self._view,"track1.png")
    # image1.scale(600,235,1)
    # image1.set_translation(Point3(400,450,0))
    # game().logic.Add(image1)
    
    # image2 = Quad(self._view,"track2.png")
    # image2.scale(600,235,1)
    # image2.set_translation(Point3(400,450,0))
    # game().logic.Add(image1)
    
    
    self.view = view
  
  def Menu_Track_1(self):
    self.parent.Pause()
    game().PushState(GameState('Track1'))

  def Menu_Track_2(self):
    self.parent.Pause()
    game().PushState(GameState('Track2'))
    
  def Menu_Back(self):
    game().PopState()

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
    MenuState.Activate(self)
    
  def Deactivate(self):
    game().simspeed = 1.
    MenuState.Deactivate(self)
    
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

