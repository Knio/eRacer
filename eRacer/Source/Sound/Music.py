from Core.Globals import *   

class Music(cpp.SoundFx):
  def __init__(self, filename):
    cpp.SoundFx.__init__(self)
    self.looping  = True
    self.is3D     = False
    self.isPaused = False
    self.volume   = 255
    self.priority = 255    
    game().sound.sound.LoadSoundFx(filename, self)
  
  def Pause(self):
    self.isPaused = True
    game().sound.sound.UpdateSoundFx(self)

  def Unpause(self):
    self.isPaused = False
    game().sound.sound.UpdateSoundFx(self)