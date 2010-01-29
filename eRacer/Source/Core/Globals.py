import os
import sys
import math

import eRacer

from Core 	import Event, Util
from Game  	import Module, Entity
from Input 	import KEY

# global game object
__game = None
def _set_game(g):
  global __game
  __game = g;
  
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

ORIGIN 		= eRacer.cvar.ORIGIN
IDENTITY 	= eRacer.cvar.IDENTITY
X       = eRacer.cvar.X
Y       = eRacer.cvar.Y
Z       = eRacer.cvar.Z



Matrix = eRacer.CreateMatrix

dot       = eRacer.dot
cross     = eRacer.cross
normalize = eRacer.normalize
mul1      = eRacer.mul1
mul0      = eRacer.mul0