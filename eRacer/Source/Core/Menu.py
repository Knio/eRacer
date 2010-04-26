from Core.Globals     import *
from Core.Config      import Config

class MenuItem(object):
  def __init__(self, label, fontsize=32, lineheight=50, enabled=True):
    self.label = label
    self.fontsize = fontsize
    self.lineheight = lineheight
    self.fontfamily = Config.FONT
    self.enabled = enabled
    
  def draw(self, view, x, y, selected):
    view.WriteString(
      self.label, self.fontfamily, self.fontsize, x, y, self.getColor(selected)
    ) 
      
    return self.lineheight 
    
  def getColor(self, selected):
    if not self.enabled:
      return GREY
    elif selected:
      return RED
    else:
      return WHITE
    

   
class ApplyMenuItem(MenuItem):
  def __init__(self, label, callback, *args, **kwargs):
    MenuItem.__init__(self, label, *args, **kwargs)
    self.callback = callback
    
  def MenuSelectEvent(self):
    self.callback()
    
    
class SelectMenuItem(MenuItem):
  def __init__(self, label, callback, options, default=0, labelwidth=250, *args, **kwargs):
    MenuItem.__init__(self,label, *args, **kwargs)
    self.callback = callback
    self.options = options
    if default < len(options):
      self.index = default	
    else:
      self.index = len(options)-1
      
    self.labelwidth = labelwidth

  def MenuLeftEvent(self):
    self.index = (self.index-1)%len(self.options)
    self.callback(self.options[self.index])
    
  def MenuRightEvent(self):
    self.index = (self.index+1)%len(self.options)
    self.callback(self.options[self.index])
    
  def draw(self, view, x, y, selected):
    MenuItem.draw(self, view, x, y, selected)
    view.WriteString(
      self.options[self.index][0], self.fontfamily, self.fontsize, x+self.labelwidth, y, self.getColor(selected)
      ) 
      
    return self.lineheight

class InputMenuItem(MenuItem):
  def __init__(self, label, callback, id, default, labelwidth=250, *args, **kwargs):
    MenuItem.__init__(self,label, *args, **kwargs)
    self.callback = callback
    self.value = default;
    self.id = id
    self.labelwidth = labelwidth

  def draw(self, view, x, y, selected):
    MenuItem.draw(self, view, x, y, selected)
    view.WriteString(
      self.value, self.fontfamily, self.fontsize, x+self.labelwidth, y, self.getColor(selected)
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

