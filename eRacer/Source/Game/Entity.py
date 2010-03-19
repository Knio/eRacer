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
  