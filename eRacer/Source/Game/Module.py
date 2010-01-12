'''
Main subsystem interface
'''


class Module(object):
  def __init__(self, game):
    self.game = game
    self.objects = []
    
  def add(self, obj):
    '''
    add an object to this module
    '''
    self.objects.append(obj)
    
    
  def start(self):
    '''
    initialize this module
    '''
    pass
    
  def tick(self, time):
    '''
    update this module
    time in microseconds since last update
    calls .tick
    '''
    for i in self.objects:
      i.tick(time)
    
  def quit(self):
    '''
    destroy this module
    '''
    pass