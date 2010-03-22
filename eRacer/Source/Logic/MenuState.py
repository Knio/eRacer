from Core.Globals     import *
from Game.State       import State
  
from Camera           import Camera, CirclingCamera, OrthographicCamera
from Quad             import Quad
from HudQuad          import HudQuad
from MenuMapping      import MainMenuMapping, PauseMenuMapping
from Graphics.View    import View


from Box import Box


class MenuState(State):
  MENU = []
  def __init__(self):
    State.__init__(self)
    self.selected = 0
    self.subSelected = {}
    for i,m in enumerate(self.MENU):
      if len(m)>1:
        self.subSelected[i] = len(m)>= 3 and m[2] or 0
    
    #width and height should not be hardcoded!
    
    camera = OrthographicCamera(800,600)
    self.Add(camera)

    self.view = View(camera,[self.scene])

    self.menuNav = cpp.SoundFx();
    self.menuNav.isLooping  = False
    self.menuNav.is3D     = False
    self.menuNav.isPaused = True
    game().sound.sound.LoadSoundFx("MenuNav.wav", self.menuNav)

    self.menuSel = cpp.SoundFx();
    self.menuSel.isLooping  = False
    self.menuSel.is3D     = False
    self.menuSel.isPaused = True
    game().sound.sound.LoadSoundFx("MenuSelect.wav", self.menuSel)
    
  def Tick(self, time):
    State.Tick(self, time)
    game().graphics.views.append(self.view)
    
    if not self.active:
      return
      
    y = 240
    for i,m in enumerate(self.MENU):
      name = m[0]
      self.view.WriteString(
        name, "Verdana", 32, Point3(100,y,0), i==self.selected and RED or WHITE
      ) 
      if len(m)>1:
        options = m[1]
        self.view.WriteString(
          options[self.subSelected[i]], "Verdana", 32, Point3(400,y,0), WHITE
        ) 
        
      y += 50
      
  def MenuUpEvent(self):
    game().sound.sound.PlaySoundFx(self.menuNav)
    self.selected = (self.selected-1) % len(self.MENU)
  
  def MenuDownEvent(self):
    game().sound.sound.PlaySoundFx(self.menuNav)
    self.selected = (self.selected+1) % len(self.MENU)

  def MenuLeftEvent(self):
    if len(self.MENU[self.selected])>1:
      game().sound.sound.PlaySoundFx(self.menuNav)
      self.subSelected[self.selected] = (self.subSelected[self.selected]-1) % len(self.MENU[self.selected][1])

  def MenuRightEvent(self):
    if len(self.MENU[self.selected])>1:
      game().sound.sound.PlaySoundFx(self.menuNav)
      self.subSelected[self.selected] = (self.subSelected[self.selected]+1) % len(self.MENU[self.selected][1])

  def MenuSelectEvent(self):
    if len(self.MENU[self.selected]) == 1:
      game().sound.sound.PlaySoundFx(self.menuSel)
      name = self.MENU[self.selected][0]
      getattr(self, 'Menu_%s' % name.replace(' ','_'))()
    
  def Menu_Exit(self):
    game().event.QuitEvent()  
    
  def SelectedOption(self,index):
    return self.MENU[index][1][self.subSelected[index]]
  

    
class MainMenuState(MenuState):
  MAPPING = MainMenuMapping
  MENU = [
    ('New Game',),
    ('Exit',)
  ]
  
  def __init__(self):
    MenuState.__init__(self)

    logo = HudQuad("Logo","eRacerXLogoNegative.png", 0, 0, 600, 235)
    logo.SetCenter(400, 150)
    self.Add(logo)
    
    self.sound = cpp.SoundFx();
    self.sound.isLooping = True
    self.sound.is3D = False
    self.sound.isPaused = False
    game().sound.sound.LoadSoundFx("Terran5.ogg", self.sound)
        
  def Pause(self):
    self.sound.isPaused = True
    game().sound.sound.UpdateSoundFx(self.sound)
    
  def Menu_New_Game(self):
    # game().PushState(GameState())
    game().PushState(GameSelectState(self.view))
    
  def Tick(self, time):
    p = Point3(500,350,0)
    for i in ['Don Ha', 'John Stuart', 'Michael Blackadar', 'Tom Flanagan', 'Ole Rehmsen']:
      self.view.WriteString(
        i, "Verdana", 28, p
      )
      p = p + Point3(0, 30, 0)
    
    MenuState.Tick(self, time)
    
    
class GameSelectState(MenuState):
  MAPPING = MainMenuMapping
  MENU = [
    ('Start',),
    ('Human Players',['1','2','4'],0),
    ('AI Players',map(str,range(8)),2),
    ('Track',['Track1','Track2'],0),
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
  
  def Menu_Start(self):
    self.parent.Pause()
    nPlayers = int(self.SelectedOption(1))
    nAIs = int(self.SelectedOption(2))
    track = self.SelectedOption(3)
    
    game().PushState(GameState(track,nPlayers,nAIs))
    
  def Menu_Back(self):
    game().PopState()

class PauseMenuState(MenuState):
  MAPPING = PauseMenuMapping
  MENU = [
    ('Continue',),
    ('Main menu',),
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

  def Menu_Main_menu(self):
    self.parent = None
    while not game().states[-1].__class__ is MainMenuState:
      game().PopState()
          
  def Tick(self, time):
    self.view.WriteString(
      "PAUSED",
      "Verdana", 40, Point3(300,100,0)
    )
    MenuState.Tick(self, time)
    self.parent.Tick(time)
      
from GameState  import GameState

