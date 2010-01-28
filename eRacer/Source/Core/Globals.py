import os
import sys
import math

import eRacer

from Core 	import Event, Util
from Game	  import Module, Entity

# global game object
__game = None
def game():
  return __game

class Vector3(eRacer.D3DXVECTOR3):
  def __repr__(self):
    return '(%6.2f, %6.2f, %6.2f)' % (self.x, self.y, self.z)

class Vector4(eRacer.D3DXVECTOR4):
  def __repr__(self):
    return '(%6.2f, %6.2f, %6.2f, %6.2f)' % (self.x, self.y, self.z, self.w)

Point3 = Vector3
Point4 = Vector4

Matrix = eRacer.CreateMatrix


