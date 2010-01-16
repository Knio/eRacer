'''
Main subsystem interface
'''


class Module(object):
  def __init__(self, game):
    self.game = game
    self.objects = []
    
  def Add(self, obj):
    '''
    add an object to this module
    '''
    self.objects.append(obj)
    
    
  def Start(self):
    '''
    initialize this module
    '''
    pass
    print 'Starting module %s' % self.__class__.__name__
    
  def Tick(self, time):
    '''
    update this module
    time in microseconds since last update
    calls .tick
    '''
    for i in self.objects:
      i.Tick(time)
    
  def Quit(self):
    '''
    destroy this module
    '''
    pass
    print 'Quitting module %s' % self.__class__.__name__