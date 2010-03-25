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
    self.players = [
      ('Player 1', Keyboard1Mapping,1)
      ]
    self.debugMappings = [KeyboardDebugMapping, GamepadDebugMapping]
    self.nAIs = 3
      
  def SetNPlayers(self, nPlayers):
    assert nPlayers==1 or nPlayers==2 or nPlayers==4


class MenuItem(object):
  def __init__(self, label):
    self.label = label
    
class ApplyMenuItem(MenuItem):
  def __init__(self, label):
    MenuItem.__init__(self,label)
    
  def Select(self):
    pass
    
class SelectMenuItem(MenuItem):
  def __init__(self, label, options, default=0):
    MenuItem.__init__(self,label)
    self.options = options
    self.index = default;

  def Prev(self):
    self.index = (self.index-1)%len(self.options)
    
  def Next(self):
    self.index = (self.index+1)%len(self.options)
    
    

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
    
  def Tick(self, time):
    State.Tick(self, time)
    game().graphics.views.append(self.view)
    
    if not self.active:
      return
      
    y = 240
    for i,m in enumerate(self.menu):
      name = m[0]
      self.view.WriteString(
        name, "Verdana", 32, Point3(100,y,0), i==self.selected and RED or WHITE
      ) 
      if len(m)>1:
        options = m[1]
        subSelected = m[2]
        self.view.WriteString(
          options[subSelected], "Verdana", 32, Point3(400,y,0), WHITE
        ) 
        
      y += 50
      
  def MenuUpEvent(self):
    game().sound.sound.PlaySoundFx(self.menuNav)
    self.selected = (self.selected-1) % len(self.menu)
  
  def MenuDownEvent(self):
    game().sound.sound.PlaySoundFx(self.menuNav)
    self.selected = (self.selected+1) % len(self.menu)

  def MenuLeftEvent(self):
    if len(self.menu[self.selected])>1:
      game().sound.sound.PlaySoundFx(self.menuNav)
      self.menu[self.selected][2] = (self.menu[self.selected][2]-1) % len(self.menu[self.selected][1])
      self.Callback(self.SelectedOption(self.selected))

  def MenuRightEvent(self):
    if len(self.menu[self.selected])>1:
      game().sound.sound.PlaySoundFx(self.menuNav)
      self.menu[self.selected][2] = (self.menu[self.selected][2]+1) % len(self.menu[self.selected][1])
      self.Callback(self.SelectedOption(self.selected))

  def MenuSelectEvent(self):
    if len(self.menu[self.selected]) == 1:
      game().sound.sound.PlaySoundFx(self.menuSel)
      self.Callback()

  def Callback(self, value = None):
    name = self.menu[self.selected][0]
    method = getattr(self, 'Menu_%s' % name.replace(' ','_'))
    value and method(value) or method()
    
    
  def Menu_Exit(self):
    game().event.QuitEvent()  
    
  def SelectedOption(self,index):
    return self.menu[index][1][self.menu[index][2]]
  

    
class MainMenuState(MenuState):
  MAPPING = MainMenuMapping
  
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
    
    self.menu = [
      ('New Game',),
      ('Exit',)
    ]
        
  def Pause(self):
    self.sound.isPaused = True
    game().sound.sound.UpdateSoundFx(self.sound)
    
  def Menu_New_Game(self):
    # game().PushState(GameState())
    game().PushState(SetupGameMenuState(self.view))
    
  def Tick(self, time):
    p = Point3(500,350,0)
    for i in ['Don Ha', 'John Stuart', 'Michael Blackadar', 'Tom Flanagan', 'Ole Rehmsen']:
      self.view.WriteString(
        i, "Verdana", 28, p
      )
      p = p + Point3(0, 30, 0)
    
    MenuState.Tick(self, time)
    
    
class SetupGameMenuState(MenuState):
  MAPPING = MainMenuMapping

  
  def __init__(self, view):
    MenuState.__init__(self)
    
    self._view = self.view
    
    self.settings = GameSettings()
    
    # image1 = Quad(self._view,"track1.png")
    # image1.scale(600,235,1)
    # image1.set_translation(Point3(400,450,0))
    # game().logic.Add(image1)
    
    # image2 = Quad(self._view,"track2.png")
    # image2.scale(600,235,1)
    # image2.set_translation(Point3(400,450,0))
    # game().logic.Add(image1)
    
    
    self.view = view
    
    self.menu = [
      ('Start',),
      ('Setup Players',),
      ('AI Players',map(str,range(8)),self.settings.nAIs),
      ('Track',['Track1','Track2'],0),
      ('Back',),
    ]
    
  def Menu_Start(self):
    self.parent.Pause() # ???
    game().PushState(GameState(self.settings))
    
    
  def Menu_AI_Players(self, value):
    self.settings.nAIs = int(value)
    
  def Menu_Track(self,value):
    self.settings.track = value
    
    
  def Menu_Setup_Players(self):
    game().PushState(SetupPlayersMenuState(self.view, self.settings))    
    
  def Menu_Back(self):
    game().PopState()
    
class SetupPlayersMenuState(MenuState):
  MAPPING = MainMenuMapping
  
  def __init__(self, view, settings):
    MenuState.__init__(self)
    
    self._view = self.view
    
    self.view = view
    self.settings = settings
    self.menu = [
      ('Human Players',['1','2','4'],0),
      ('Back',),
    ]

  def Menu_Human_Players(self, value):
    nPlayers = int(value)
    self.settings.debugMappings = nPlayers > 1 and [] or [KeyboardDebugMapping, GamepadDebugMapping]
    
    while len(self.settings.players) < nPlayers:
      name = 'Player %d' % (len(self.players)+1)
      self.menu.insert(len(self.menu)-1, ('Player name',))

      mappingIndex = len(self.freeMappingIndices)>0 and self.freeMappingIndices.pop() or None
      mapping = self.availableMappings[mappingIndex]
      self.menu.insert(len(self.menu)-1, ('Controls', str(mapping),mappingIndex))
      
      textureIndex = self.freeTextureIndices.pop()
      textureId = self.textureIds[textureIndex]
      self.menu.insert(len(self.menu)-1, ('Colors', self.textureStrings,textureIndex))
      
      self.players.append((name, mapping, textureId))
      
    while len(self.players) > nPlayers:
      self.menu.pop(len(self.menu)-1)
      self.menu.pop(len(self.menu)-1)
      self.menu.pop(len(self.menu)-1)

      self.players.pop()
 
  
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
      "Verdana", 40, Point3(300,100,0)
    )
    MenuState.Tick(self, time)
    self.parent.Tick(time)
      
from GameState  import GameState

