from Core.Globals     import *

class MenuItem(object):
  def __init__(self, label):
    self.label = label
    self.fontsize = 32
    self.lineheight = 50
    
  def draw(self, view, position, selected):
    view.WriteString(
      self.label, "Sony Sketch EF", self.fontsize, position, selected and RED or WHITE
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
      self.options[self.index][0], "Sony Sketch EF", self.fontsize, position+Point3(300,0,0), WHITE
      ) 
      
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
      
    return self.lineheight