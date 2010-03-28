from Core.Globals     import *
from GameMapping      import *

class GameSettings(object):
  PLAYER_NUMS = [1,2,4]
  TRACKS = ['Track1', 'Track2']
  MAPPINGS = [None, 
              Keyboard1Mapping, 
              Keyboard2Mapping, 
              Gamepad1Mapping, 
              Gamepad2Mapping, 
              Gamepad3Mapping, 
              Gamepad4Mapping, 
             ]
  TEXTURE_IDS = [1,2,3,4,5,6,8]
  TEXTURE_NAMES = ['Blue', 'Red', 'Magenta', 'Cyan', 'Green', 'Black', 'Orange']
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

  def __init__(self):
    self.freeTextureIndices = set()
    self.trackIndex = 0
    self.playersIndices = []
    self.debugMappings = []
    self.nPlayersIndex = 0
    self.nAIs = 3
    self.nTotalLaps = 2
    
  def ResetFreeTextures(self):
    self.freeTextureIndices = set(range(len(self.TEXTURE_IDS)))
    for player in self.playersIndices:
      self.freeTextureIndices.discard(player.textureIndex)
    
  
  def RandomTextureId(self):
    if len(self.freeTextureIndices)==0:
        self.ResetFreeTextures()
    
    return self.TEXTURE_IDS[self.freeTextureIndices.pop()]
    
  def get_track(self):
    return self.TRACKS[self.trackIndex]
  track = property(get_track)

  def get_players(self):
    return [Struct(name = player.name, 
                   mapping=self.MAPPINGS[player.mappingIndex], 
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
      player.name = 'Player %d' % (playerId+1)
      player.mappingIndex = playerId+1
      player.textureIndex = playerId
      
      self.playersIndices.append(player)
    
    while len(self.playersIndices) > nPlayers:
      self.playersIndices.pop()
  nPlayersIndex = property(get_n_players_index, set_n_players_index)    