from Core.Globals import *

class Window(cpp.Window):
  def __init__(self, title):
    cpp.Window.__init__(self, CONSTS.SCREEN_WIDTH, CONSTS.SCREEN_HEIGHT)
    self.Create(title)
    self.SetTitle("eRacerX")
    