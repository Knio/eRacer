from Core.Globals     import *
from Game.State       import State
  
from Camera           import Camera, CirclingCamera, OrthographicCamera
from Quad             import Quad
from HudQuad          import HudQuad
from MenuMapping      import MainMenuMapping, PauseMenuMapping
from GameMapping      import *
from Graphics.View    import View


from Box import Box

class GameSettings(object):
  def __init__(self):
    self.freeTextureIds = [1,2,3,4,5,6,8]
    self.track = 'Track1'
    self.players = []
    self.debugMappings = []
    self.nAIs = 3



class MenuItem(object):
  def __init__(self, label):
    self.label = label
    self.fontsize = 32
    self.lineheight = 50
    
  def draw(self, view, position, selected):
    view.WriteString(
      self.label, "Sony Sketch EF", self.fontsize, position, selected and RED or WHITE
    ) 
      
    #return height  
    return self.lineheight 
    
class ApplyMenuItem(MenuItem):
  def __init__(self, label, callback):
    MenuItem.__init__(self,label)
    self.callback = callback
    
  def MenuSelectEvent(self):
    self.callback()
    
    
class SelectMenuItem(MenuItem):
  def __init__(self, label, callback, options, default=0):
    MenuItem.__init__(self,label)
    self.callback = callback
    self.options = options
    self.index = default;

  def MenuLeftEvent(self):
    self.index = (self.index-1)%len(self.options)
    self.callback(self.options[self.index])
    
  def MenuRightEvent(self):
    self.index = (self.index+1)%len(self.options)
    self.callback(self.options[self.index])
    
  def draw(self, view, position, selected):
    MenuItem.draw(self, view, position, selected)
    view.WriteString(
      self.options[self.index][0], "Sony Sketch EF", self.fontsize, position+Point3(300,0,0), WHITE
      ) 
      
    #return height  
    return self.lineheight
    
class InputMenuItem(MenuItem):
  def __init__(self, label, callback, default):
    MenuItem.__init__(self,label)
    self.callback = callback
    self.value = default;

  def draw(self, view, position, selected):
    MenuItem.draw(self, view, position, selected)
    view.WriteString(
      self.value, "Sony Sketch EF", self.fontsize, position+Point3(300,0,0), WHITE
      ) 
      
    #return height  
    return self.lineheight
    

class MenuState(State):
  def __init__(self):
    State.__init__(self)
    self.selected = 0
    
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

    logo = HudQuad("Logo","eRacerXLogoNegative.png", 0, 0, 600, 235)
    logo.SetLeftTop(30, 35)
    self.Add(logo)
    
    self.sound = cpp.SoundFx();
    self.sound.isLooping = True
    self.sound.is3D = False
    self.sound.isPaused = False
    game().sound.sound.LoadSoundFx("Terran5.ogg", self.sound)
    
    self.menu = [
      ApplyMenuItem('New Game', self.Menu_New_Game),
      ApplyMenuItem('Exit', self.Menu_Exit)
    ]
    self.menuTop = 240

        
  def Pause(self):
    self.sound.isPaused = True
    game().sound.sound.UpdateSoundFx(self.sound)
    
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
      SelectMenuItem('Track', self.Menu_Track, [('Track1',),('Track2',)], 0),
      ApplyMenuItem('Back', self.Menu_Back),
    ]
    
  def Menu_Start(self):
    self.parent.Pause() # ???
    game().PushState(GameState(self.settings))
        
  def Menu_AI_Players(self, value):
    self.settings.nAIs = value[1]
    
  def Menu_Track(self,value):
    self.settings.track = value    
    
  def Menu_Setup_Players(self):
    game().PushState(SetupPlayersMenuState(self.settings))    
    
  def Menu_Back(self):
    game().PopState()
    
class SetupPlayersMenuState(MenuState):
  MAPPING = MainMenuMapping
  
  def __init__(self, settings):
    MenuState.__init__(self)
    
    
    humanPlayerOptions = []
    for i in [1,2,4]:
      humanPlayerOptions.append((str(i),i))    
    
    self.settings = settings
    self.menu = [
      SelectMenuItem('Human Players',self.Menu_Human_Players, humanPlayerOptions, 0),
      ApplyMenuItem('Back',self.Menu_Back),
    ]
    
    self.availableMappings = [None, 
                              Keyboard1Mapping, 
                              Keyboard2Mapping, 
                              Gamepad0Mapping, 
                              Gamepad1Mapping, 
                              Gamepad2Mapping, 
                              Gamepad3Mapping, 
                              ]
    
    
    self.textureIds = [1,2,3,4,5,6,8]
    
    self.textureMap = {}
    self.textureMap[1] = "Blue"
    self.textureMap[2] = "Red"
    self.textureMap[3] = "Magenta"
    self.textureMap[4] = "Cyan"
    self.textureMap[5] = "Green"
    self.textureMap[6] = "Black"
    self.textureMap[8] = "Orange"
    
    self.Menu_Human_Players(('1',1))    

  def Menu_Human_Players(self, value):
    nPlayers = value[1]
    self.settings.debugMappings = nPlayers > 1 and [] or [KeyboardDebugMapping, GamepadDebugMapping]
    
    fontsize = 24
    lineheight = 24
    padding = 10
    
    while len(self.settings.players) < nPlayers:
      playerId = len(self.settings.players)
      name = 'Player %d' % (playerId+1)
      self.menu.insert(len(self.menu)-1, InputMenuItem('Player name',self.Menu_Player_Name, name))

      self.menu[-2].fontsize = fontsize
      self.menu[-2].lineheight = lineheight

      mapping = self.availableMappings[playerId]
      
      mappingOptions = []
      for mapping in self.availableMappings:
        mappingOptions.append((str(mapping),playerId,mapping))
        
      self.menu.insert(len(self.menu)-1, SelectMenuItem('Controls', 
                                                self.Menu_Controls, 
                                                mappingOptions, 
                                                playerId+1))

      self.menu[-2].fontsize = fontsize
      self.menu[-2].lineheight = lineheight

      
      textureOptions = []

      for textureId in self.textureIds:
        textureOptions.append((self.textureMap[textureId], playerId, textureId))
      
      textureId = self.textureIds[playerId]
      self.menu.insert(len(self.menu)-1, SelectMenuItem('Color', 
                                            self.Menu_Color,
                                            textureOptions,
                                            playerId))

      self.menu[-2].fontsize = fontsize
      self.menu[-2].lineheight = lineheight+padding
      
      self.settings.players.append((name, mapping, textureId))
      
    while len(self.settings.players) > nPlayers:
      self.menu.pop(len(self.menu)-2)
      self.menu.pop(len(self.menu)-2)
      self.menu.pop(len(self.menu)-2)

      p = self.settings.players.pop()

 
  
  def Menu_Player_Name(self, value):
    pass
    
  def Menu_Controls(self, value):
    pass
    
  def Menu_Color(self, value):
    pass
  
  
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
          
  def Tick(self, time):
    self.view.WriteString(
      "PAUSED",
      "Sony Sketch EF", 40, Point3(300,100,0)
    )
    MenuState.Tick(self, time)
    self.parent.Tick(time)
      
from GameState  import GameState

