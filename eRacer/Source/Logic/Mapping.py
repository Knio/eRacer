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
  def __getattribute__(self, attr):
    method = object.__getattribute__(self, attr)
    if not method or not attr.endswith('Event'): return method
    def f(*args):
      eventlist = method(*args)
      if not eventlist: return
      if not isinstance(eventlist[0], tuple):
        eventlist = [eventlist]
      for event,args in eventlist:
        event(*args)
    return f


