import eRacer

class Window(eRacer.Window):
  def __init__(self, title):
    eRacer.Window.__init__(self)
    self.Create(title)
    