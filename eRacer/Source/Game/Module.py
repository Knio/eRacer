'''
Main subsystem interface
'''

import eRacer

class Module(eRacer.Module):
  def __init__(self):
    eRacer.Module.__init__(self)
    self.objects = []
        
  def Init(self):
    '''
    Initialize this module.
    Here you can assume that all of the
    game subsystems have been constructed
    '''
    pass
    # print 'Initializing module %s' % self.__class__.__name__    
    
  def Start(self):
    '''
    initialize this module
    '''
    # print 'Starting module %s' % self.__class__.__name__
    
  def Tick(self, time):
    '''
    update this module
    time in microseconds since last update
    calls .tick
    '''
    #print 'Ticking module %s' % self.__class__.__name__
    pass
            
  def Quit(self):
    '''
    destroy this module
    '''
    # print 'Quitting module %s' % self.__class__.__name__