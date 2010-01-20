import eRacer

class Event(eRacer.Event):
  def __init__(self, game):
    eRacer.Event.__init__(self)
    self.listeners = {}
    print 'Init!'
        
  def Register(self, func, obj):
    print 'Register', func, obj
    self.listeners.setdefault(func, []).append(obj)

  def __getattribute__(self, attr):
    if attr.endswith('Event'):
      def f(*args, **kwargs):
        print '%s%r%r' % (attr, args, kwargs)
        for i in self.listeners.get(attr, []):
          try:
            getattr(i, attr)(*args, **kwargs)
          except:
            import traceback
            traceback.print_exc()
        return 0
      return f
      
    return object.__getattribute__(self, attr)

