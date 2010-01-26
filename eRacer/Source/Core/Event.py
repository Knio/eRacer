import eRacer

class Event(eRacer.Event):
  def __init__(self, game):
    eRacer.Event.__init__(self)
    self.listeners = {}

  def Register(self, obj, event=None):
    print 'Register', event, obj
    if callable(obj):
      event = obj.__name__
      func = obj
    else:
      func = getattr(obj, event)
    self.listeners.setdefault(event, []).append(func)

  def __getattribute__(self, attr):
    if attr.endswith('Event'):
      def f(*args, **kwargs):
        print '%s%r%r' % (attr, args, kwargs)
        for f in self.listeners.get(attr, []):
          try:
            f(*args, **kwargs)
          except:
            import traceback
            print 'Error calling listener callback: %s %r' % (attr, f)
            traceback.print_exc()
        return 0
      return f

    return object.__getattribute__(self, attr)
