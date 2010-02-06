'''
Game entity object interface
'''

from Core.Globals import *


class Entity(object):
  def __init__(self):
    self.__transform = Matrix()
	
    
  def Tick(self, time):
    pass
  
  def get_transform(self):
  	return self.__transform

  def get_translation(self):
    return eRacer.GetTranslation(self.transform)

  def transform_changed(self):
    pass
  
  def set_transform(self, transform):
    self.__transform = transform
    self.transform_changed()
      
  def set_translation(self, translation):
    eRacer.SetTranslation(self.__transform, translation)
    self.transform_changed()
    
  def translate(self, translation):
    eRacer.Translate(self.__transform, translation)
    self.transform_changed()
    
  transform = property(get_transform, set_transform)
 
    
  
    
    
