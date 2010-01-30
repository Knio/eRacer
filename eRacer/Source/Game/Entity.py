'''
Game entity object interface
'''
from Core.Globals import *

class Entity(object):
  def __init__(self, game):
	self.game = game
	self.__transform = eRacer.CreateMatrix(Vector3(0, 0, 0))
	
    
  def Tick(self, time):
    pass
  
  def get_transform(self):
  	return __transform
  
  def set_transform(self, transform):
  	self.__transform = transform
  
  transform = property(get_transform, set_transform)