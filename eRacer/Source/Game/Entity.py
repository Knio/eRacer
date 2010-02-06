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

  def transform_changed(self):
    pass
  

  def get_translation(self):
    return eRacer.GetTranslation(self.transform)

  def set_transform(self, transform):
    self.__transform = transform
    self.transform_changed()
      
  def set_translation(self, translation):
    eRacer.SetTranslation(self.__transform, translation)
    self.transform_changed()

  #def set_translation(self, x, y, z):
  #  self.set_translation(Vector3(x,y,z))

    
  def translate(self, translation):
    eRacer.Translate(self.__transform, translation)
    self.transform_changed()
 
  def set_rotation(self, yaw, pitch, roll):
    eRacer.SetRotation(self.__transform, yaw, pitch, roll)
    self.transform_changed()
    
  def rotate(self, yaw, pitch, roll):
    eRacer.Rotate(self.__transform,yaw, pitch, roll)
    self.transform_changed()
 
    
  transform = property(get_transform, set_transform)
 
    
  
    
    
