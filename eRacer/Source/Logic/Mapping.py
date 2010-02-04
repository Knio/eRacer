from Core.Globals import *

class event(object):
  def __getattr__(self, attr):
    if not attr.endswith('Event'):
      return object.__getattribute__(self, attr)
    def f(*args):
      return getattr(game().event, attr), args
    return f
E = event()

class Mapping(object):
  def __init__(self):
    self.MAP = {}

  def __getattr__(self, attr):
    if not attr.endswith('Event'):
      return object.__getattribute__(self, attr)
    
    def f(*args):    
      mapping = self.MAP.get(attr)
      if not mapping: return
        
      if isinstance(mapping, dict):
        eventlist = mapping.get(args)
        if not eventlist: return
                      
      else:
        eventlist = mapping(*args)

      for event,args in eventlist:
        event(*args)
    
    return f
      
      
class GameMapping(Mapping):
  def __init__(self):
    from GameMapping import MAP
    self.MAP = MAP
    

    
    
      
      
      
      
      
      
    
    


    
    
    
    
  
    
  
    
  









