import eRacer

class Event(eRacer.Event):
  def __init__(self, game):
    eRacer.Event.__init__(self)
    print 'Init!'
        
  def Register(self, func, obj):
    print 'Register', func, obj

  def __getattribute__(self, attr):
    if attr.endswith('Event'):
      def f(*args):
        print '%s%r' % (attr, args)
        return 0
      return f
    
    return object.__getattribute__(self, attr)

