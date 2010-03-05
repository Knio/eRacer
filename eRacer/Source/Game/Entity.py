'''
Game entity object interface
'''

from Core.Globals import *


class Entity(object):
  idCounter = 0
  entities = {}
  
  def __init__(self):
    Entity.idCounter+=1
    self.id = Entity.idCounter
    self.entities[self.id] = self	
    
  def Tick(self, time):
    pass
  
    
    
