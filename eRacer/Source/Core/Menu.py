from Core.Globals     import *
from Core.Config      import Config

class MenuItem(object):
  def __init__(self, label, fontsize=32, lineheight=50):
    self.label = label
    self.fontsize = fontsize
    self.lineheight = lineheight
    self.fontfamily = Config.FONT
    
  def draw(self, view, x, y, selected, width=-1):
    view.WriteString(
      self.label, self.fontfamily, self.fontsize, x, y, selected and RED or WHITE
    ) 
      
    return self.lineheight 
    
class ApplyMenuItem(MenuItem):
  def __init__(self, label, callback, *args, **kwargs):
    MenuItem.__init__(self, label, *args, **kwargs)
    self.callback = callback
    
  def MenuSelectEvent(self):
    self.callback()
    
    
class SelectMenuItem(MenuItem):
  def __init__(self, label, callback, options, default=0):
    MenuItem.__init__(self,label)
    self.callback = callback
    self.options = options
    if default < len(options):
      self.index = default
    else:
      self.index = len(options)-1

  def MenuLeftEvent(self):
    self.index = (self.index-1)%len(self.options)
    self.callback(self.options[self.index])
    
  def MenuRightEvent(self):
    self.index = (self.index+1)%len(self.options)
    self.callback(self.options[self.index])
    
  def draw(self, view, x, y, selected, width=300):
    MenuItem.draw(self, view, x, y, selected)
    view.WriteString(
      self.options[self.index][0], self.fontfamily, self.fontsize, x+width, y, WHITE
      ) 
      
    return self.lineheight
    
class InputMenuItem(MenuItem):
  def __init__(self, label, callback, id, default):
    MenuItem.__init__(self,label)
    self.callback = callback
    self.value = default;
    self.id = id

  def draw(self, view, x, y, selected, width=300):
    MenuItem.draw(self, view, x, y, selected)
    view.WriteString(
      self.value, self.fontfamily, self.fontsize, x+width, y, selected and RED or WHITE
      ) 
      
    return self.lineheight
    
  def CharacterTypedEvent(self, character):
    if character.isalnum() or character.isspace():
      self.value += character
      self.callback(self.id, self.value)
    
  def KeyPressedEvent(self, key):
    if key == KEY.BACK:
      self.value = self.value[:len(self.value)-1]
      self.callback(self.id, self.value)    
