from Core.Globals     import *
from GameMapping      import *

import pkgutil 
 
import Tracks



class GameSettings(object):
  PLAYER_NUMS = [1,2,3,4]
  
  KEYBOARD_MAPPINGS = [
                        Keyboard1Mapping, 
                        Keyboard2Mapping, 
                      ]
             
  GAMEPAD_MAPPINGS =  [
                        Gamepad1Mapping, 
                        Gamepad2Mapping, 
                        Gamepad3Mapping, 
                        Gamepad4Mapping, 
                      ]
  TEXTURE_IDS = [1,2,3,4,5,6,7,8,9,10]
  
  TEXTURE_NAMES = ['Blue', 'Red', 'Green', 'Yellow', 'Orange', 'Magenta', 'Black', 'Grey', 'Cyan', 'White']
  
  MAX_AIS = 8
  
  AI_NAMES = [
    "Arthur Dent", 
    "Ford Prefect", 
    "Zaphod", 
    "Marvin", 
    "Trillian", 
    "Slartibartfast",
    "Philip J. Fry",
    "Bender",
    "Turanga Leela"
  ]
  
  LAP_COUNTS = range(1,6)

  def __init__(self):
    self.availableMappings = [None]
    self.availableMappings.extend(self.GAMEPAD_MAPPINGS[:game().input.GetNumGamepads()])
    if game().input.HasKeyboard():
      self.availableMappings.extend(self.KEYBOARD_MAPPINGS)
    
    self.availablePlayerNums = []
    for num in self.PLAYER_NUMS:
      if num < len(self.availableMappings) or game().debug:
        self.availablePlayerNums.append(num)
    
    self.freeTextureIndices = set()
    self.playersIndices = []
    self.debugMappings = []
    self.nPlayersIndex = 0

    self.availableTracks = []
    self.availableTrackNames = []
    
    for importer, modname, ispkg in pkgutil.iter_modules(Tracks.__path__): 
      self.availableTracks.append(modname)
      track = __import__("Tracks."+modname,fromlist=['NAME'])
      self.availableTrackNames.append(hasattr(track,'NAME') and track.NAME or modname)
      
        
    
    
  def ResetFreeTextures(self):
    self.freeTextureIndices = set(range(len(self.TEXTURE_IDS)))
    for player in self.playersIndices:
      self.freeTextureIndices.discard(player.textureIndex)
    
  
  def RandomTextureId(self):
    if len(self.freeTextureIndices)==0:
        self.ResetFreeTextures()
    
    return self.TEXTURE_IDS[self.freeTextureIndices.pop()]
    
  def get_track(self):
    return self.availableTracks[self.trackIndex]
  track = property(get_track)

  def get_players(self):
    return [Struct(name = player.name, 
                   mapping=self.availableMappings[player.mappingIndex], 
                   textureId=self.TEXTURE_IDS[player.textureIndex]) for player in self.playersIndices]
  players = property(get_players)
  
  def get_n_players(self):
    return len(self.playersIndices)
  
  nPlayers = property(get_n_players)    
  
  def get_n_players_index(self):
    return self._nPlayersIndex
    
  def set_n_players_index(self, nPlayersIndex):
    self._nPlayersIndex = nPlayersIndex
    nPlayers = self.PLAYER_NUMS[nPlayersIndex]
    self.debugMappings = nPlayers > 1 and [] or [KeyboardDebugMapping, GamepadDebugMapping]
    
    
    while len(self.playersIndices) < nPlayers:
      playerId = len(self.playersIndices)
      
      player = Struct()
      player.name = game().config.get_setting('name', 'PLAYER%d'%(playerId+1))

      player.mappingIndex = playerId+1
      if player.mappingIndex>=len(self.availableMappings):
        player.mappingIndex = 0

      player.textureIndex = int(game().config.get_setting('textureIndex', 'PLAYER%d'%(playerId+1)))
      
      self.playersIndices.append(player)
    
    while len(self.playersIndices) > nPlayers:
      self.playersIndices.pop()
  nPlayersIndex = property(get_n_players_index, set_n_players_index)  
  
  def get_num_laps(self):
    return self.LAP_COUNTS[self.nLapsIndex]
    
  nLaps = property(get_num_laps)

  def get_num_laps_index(self):
    return int(game().config.get_setting('nLapsIndex'))
  
  def set_num_laps_index(self,nLapsIndex):
    game().config.set_setting('nLapsIndex',str(nLapsIndex))
    
  nLapsIndex = property(get_num_laps_index,set_num_laps_index)  

  def get_track_index(self):
    return int(game().config.get_setting('trackIndex'))
  
  def set_track_index(self,trackIndex):
    game().config.set_setting('trackIndex',str(trackIndex))
    
  trackIndex = property(get_track_index,set_track_index)  

  
  
  def get_n_ais(self):
    return int(game().config.get_setting('nAIs'))
  
  def set_n_ais(self,nAIs):
    game().config.set_setting('nAIs',str(nAIs))
    
  nAIs = property(get_n_ais,set_n_ais)  

  
  def set_player_name(self, id, name):
    self.playersIndices[id].name = name 
    game().config.set_setting('name', name,'PLAYER%d'%(id+1))
    
  def set_player_texture_index(self, id, textureIndex):
    self.playersIndices[id].textureIndex = textureIndex 
    game().config.set_setting('textureIndex', str(textureIndex),'PLAYER%d'%(id+1))  