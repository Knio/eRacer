from Core.Globals     import *
from Core.Menu        import *
from Game.State       import State
  
from Camera           import Camera, CirclingCamera, OrthographicCamera
from Quad             import Quad
from HudQuad          import HudQuad
from MenuMapping      import *
from Graphics.View    import View, HudView
from GameSettings     import GameSettings  


class MenuState(State):
  def __init__(self):
    State.__init__(self)
    self.selected = 0
    
    self.view = HudView([self.scene])

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
    self.menu = []
    self.menuTop = 50
    
  def Tick(self, time):
    State.Tick(self, time)
    game().graphics.views.append(self.view)
    
    if not self.active:
      return
      
    position = Point3(100,self.menuTop,0)  

    for i,m in enumerate(self.menu):
      yOffset = m.draw(self.view,position, i == self.selected)
      position.y += yOffset
      
  def MenuUpEvent(self):
    game().sound.sound.PlaySoundFx(self.menuNav)
    self.selected = (self.selected-1) % len(self.menu)
  
  def MenuDownEvent(self):
    game().sound.sound.PlaySoundFx(self.menuNav)
    self.selected = (self.selected+1) % len(self.menu)

  def MenuLeftEvent(self):
    method = getattr(self.menu[self.selected], 'MenuLeftEvent', None)
    if method: 
      game().sound.sound.PlaySoundFx(self.menuNav)
      method()

  def MenuRightEvent(self):
    method = getattr(self.menu[self.selected], 'MenuRightEvent', None)
    if method: 
      game().sound.sound.PlaySoundFx(self.menuNav)
      method()

  def MenuSelectEvent(self):
    method = getattr(self.menu[self.selected], 'MenuSelectEvent', None)
    if method: 
      game().sound.sound.PlaySoundFx(self.menuSel)
      method()

  def Menu_Exit(self):
    game().event.QuitEvent()  
    
    
class MainMenuState(MenuState):
  MAPPING = MainMenuMapping
  
  def __init__(self):
    MenuState.__init__(self)

    logo = HudQuad("Logo","eRacerXLogoNegative.png", 30, 35, 600, 235)
    self.view.Add(logo)
    
    self.LoadMusic("Terran5.ogg")
    
    self.menu = [
      ApplyMenuItem('New Game', self.Menu_New_Game),
      ApplyMenuItem('Exit', self.Menu_Exit)
    ]
    self.menuTop = 240

        

    
  def Menu_New_Game(self):
    # game().PushState(GameState())
    game().PushState(SetupGameMenuState())
    
  def Tick(self, time):
    p = Point3(500,350,0)
    for i in ['Don Ha', 'John Stuart', 'Michael Blackadar', 'Tom Flanagan', 'Ole Rehmsen']:
      self.view.WriteString(
        i, "Sony Sketch EF", 28, p
      )
      p = p + Point3(0, 30, 0)
    
    MenuState.Tick(self, time)
    
    
class SetupGameMenuState(MenuState):
  MAPPING = MainMenuMapping

  
  def __init__(self):
    MenuState.__init__(self)
    
    self.settings = GameSettings()
    
    # image1 = Quad(self._view,"track1.png")
    # image1.scale(600,235,1)
    # image1.set_translation(Point3(400,450,0))
    # game().logic.Add(image1)
    
    # image2 = Quad(self._view,"track2.png")
    # image2.scale(600,235,1)
    # image2.set_translation(Point3(400,450,0))
    # game().logic.Add(image1)
    
    
    aiPlayerOptions = []
    for i in range(8):
      aiPlayerOptions.append((str(i),i))
    
    self.menu = [
      ApplyMenuItem('Start', self.Menu_Start),
      ApplyMenuItem('Setup Players', self.Menu_Setup_Players),
      SelectMenuItem('AI Players', self.Menu_AI_Players, aiPlayerOptions, self.settings.nAIs),
      SelectMenuItem('Track', self.Menu_Track, [('Triple Eight',0),('Cyclone',1)], 0),
      ApplyMenuItem('Back', self.Menu_Back),
    ]
    
  def Menu_Start(self):
    self.parent.PauseMusic()
    game().PushState(GameState(self.settings))
        
  def Menu_AI_Players(self, value):
    self.settings.nAIs = value[1]
    
  def Menu_Track(self,value):
    self.settings.trackIndex = value[1]    
    
  def Menu_Setup_Players(self):
    game().PushState(SetupPlayersMenuState(self.settings))    
    
  def Menu_Back(self):
    game().PopState()
    
class SetupPlayersMenuState(MenuState):
  MAPPING = MainMenuMapping
  
  def __init__(self, settings):
    MenuState.__init__(self)  
    
    self.settings = settings
   
    self.UpdateMenu()    

  def UpdateMenu(self):
    self.menu = []

    humanPlayerOptions = []
    for i,num in enumerate(GameSettings.PLAYER_NUMS):
      humanPlayerOptions.append((str(num),i))   

    self.menu.append(SelectMenuItem('Human Players', self.Menu_Human_Players, humanPlayerOptions, self.settings.nPlayersIndex))
    
    fontsize = 24
    lineheight = 24
    padding = 10

    for playerId,player in enumerate(self.settings.playersIndices):
      self.menu.append(InputMenuItem('Name', self.Menu_Name, player.name))
      self.menu[-1].fontsize = fontsize
      self.menu[-1].lineheight = lineheight
    
      mappingOptions = []
      for i,mapping in enumerate(self.settings.availableMappings):
        s = mapping and mapping.__name__.replace('Mapping','') or 'None'
        mappingOptions.append((s,playerId,i))   
    
      self.menu.append(SelectMenuItem('Controls', self.Menu_Controls, mappingOptions, player.mappingIndex))
      self.menu[-1].fontsize = fontsize
      self.menu[-1].lineheight = lineheight
      
      textureOptions = []

      for i,textureId in enumerate(GameSettings.TEXTURE_IDS):
        textureOptions.append((GameSettings.TEXTURE_NAMES[i], playerId, i))
      
      self.menu.append(SelectMenuItem('Color', self.Menu_Color, textureOptions, player.textureIndex))
      self.menu[-1].fontsize = fontsize
      self.menu[-1].lineheight = lineheight + padding

    
    self.menu.append(ApplyMenuItem('Back', self.Menu_Back))
    

  def Menu_Human_Players(self, value):
    self.settings.nPlayersIndex = value[1]      
    self.UpdateMenu()

  def Menu_Name(self, value):
    self.settings.playersIndices[value[1]].name = value[2] 
    
  def Menu_Controls(self, value):
    self.settings.playersIndices[value[1]].mappingIndex = value[2]
    
  def Menu_Color(self, value):
    self.settings.playersIndices[value[1]].textureIndex = value[2]
  
  
  def Menu_Back(self):
    game().PopState()    

class PauseMenuState(MenuState):
  MAPPING = PauseMenuMapping
  
  def __init__(self):
    MenuState.__init__(self)
    self.menu = [
      ApplyMenuItem('Continue',self.Menu_Continue),
      ApplyMenuItem('Restart race', self.Menu_Restart_race),
      ApplyMenuItem('Main menu',self.Menu_Main_menu),
      ApplyMenuItem('Exit',self.Menu_Exit),
    ]
    self.menuTop = 240
    
  def Activate(self):
    print "activate pause!!!!"
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
      
  def Menu_Restart_race(self):
    while not game().states[-1].__class__ is GameState:
      game().PopState()
    
    gamestate = game().states[-1]
    gamestate.Release()
    gamestate.load(gamestate.settings)
          
  def Tick(self, time):
    self.view.WriteString(
      "PAUSED",
      "Sony Sketch EF", 40, Point3(300,100,0)
    )
    MenuState.Tick(self, time)
    self.parent.Tick(time)
      
from GameState  import GameState

