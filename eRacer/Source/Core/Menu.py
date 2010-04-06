from Core.Globals     import *

class MenuItem(object):
  def __init__(self, label):
    self.label = label
    self.fontsize = 32
    self.lineheight = 50
    self.fontfamily = "Sony Sketch EF"
    
  def draw(self, view, position, selected):
    view.WriteString(
      self.label, self.fontfamily, self.fontsize, position, selected and RED or WHITE
    ) 
      
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
      self.options[self.index][0], self.fontfamily, self.fontsize, position+Point3(300,0,0), WHITE
      ) 
      
    return self.lineheight
    
class InputMenuItem(MenuItem):
  def __init__(self, label, callback, id, default):
    MenuItem.__init__(self,label)
    self.callback = callback
    self.value = default;
    self.active = False
    self.id = id
    game().event.Register(self.CharacterTypedEvent)
    game().event.Register(self.KeyPressedEvent)

  def draw(self, view, position, selected):
    MenuItem.draw(self, view, position, selected)
    view.WriteString(
      self.value, self.fontfamily, self.fontsize, position+Point3(300,0,0), self.active and RED or WHITE
      ) 
      
    return self.lineheight
    
  def MenuSelectEvent(self):
    self.active = not self.active
    self.active or self.callback(self.id, self.value)
    
  def CharacterTypedEvent(self, character):
    if self.active and character.isalnum() or character.isspace():
      self.value += character
    
  def KeyPressedEvent(self, key):
    if self.active:
      if key == KEY.BACK:
        self.value = self.value[:len(self.value)-1]
    
