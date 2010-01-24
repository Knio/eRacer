'''
Main subsystem interface
'''

import eRacer

class Module(eRacer.Module):
  def __init__(self, game):
    eRacer.Module.__init__(self)
    self.game = game
    self.objects = []
    
  def Add(self, obj):
    '''
    add an object to this module
    '''
    self.objects.append(obj)
    
  def Init(self):
    '''
    Initialize this module.
    Here you can assume that all of the
    game subsystems have been constructed
    '''
    pass
    print 'Initializing module %s' % self.__class__.__name__    
    
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