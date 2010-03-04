'''
Game entity object interface
'''

from Core.Globals import *


class Entity(object):
  idCounter = 0
  entities = {}
  
  def __init__(self):
    self.__class__.idCounter+=1
    self.id = self.idCounter
    self.entities[self.id] = self	
    
  def Tick(self, time):
    pass
  
    
    
