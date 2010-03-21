'''
Game entity object interface
'''

from Core.Globals import *


class Entity(object):
  idCounter = 0
  
  def __init__(self):
    Entity.idCounter+=1
    self.id = Entity.idCounter
    
  def Tick(self, time):
    pass
  
  def Release(self):
    pass
  
  def __del__(self):
    print 'del %d %r' % (self.id, self)