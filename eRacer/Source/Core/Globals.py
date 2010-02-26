import os
import sys
import math

import eRacer

# global game object
__game = None
def _set_game(g):
  global __game
  __game = g;
  
def game():
  return __game


def VECTOR3__iadd__(self, arg):
  raise Exception("The (+=) operator seems to be unstable. Please do not use.")

def VECTOR3__repr__(self):
  return "(%6.2f %6.2f %6.2f)" % (self.x, self.y, self.z)

eRacer.D3DXVECTOR3.__iadd__ = VECTOR3__iadd__
eRacer.D3DXVECTOR3.__repr__ = VECTOR3__repr__


Vector3   = eRacer.D3DXVECTOR3
Vector4   = eRacer.D3DXVECTOR4
Point3    = eRacer.D3DXVECTOR3
Point4    = eRacer.D3DXVECTOR4

Matrix    = eRacer.CreateMatrix

ORIGIN 		= eRacer.cvar.ORIGIN
IDENTITY 	= eRacer.cvar.IDENTITY
X         = eRacer.cvar.X
Y         = eRacer.cvar.Y
Z         = eRacer.cvar.Z
RED       = eRacer.cvar.RED
BLUE      = eRacer.cvar.BLUE
GREEN     = eRacer.cvar.GREEN
WHITE     = eRacer.cvar.WHITE

length    = eRacer.abs
dot       = eRacer.dot
cross     = eRacer.cross
normalize = eRacer.normalize
normalized= eRacer.normalized
normal    = eRacer.normalized
project   = eRacer.project
mul1      = eRacer.mul1
mul0      = eRacer.mul0

CreateMatrix = eRacer.CreateMatrix

PI    = math.pi
ZERO  = 1e-5
def printvec(v):
  print "(%6.2f %6.2f %6.2f)" % (v.x, v.y, v.z)

def clamp(x, _min, _max):
  return min(max(x, _min), _max)

CONSTS    = eRacer.Constants().g_Constants

from Game.Module  	import Module
from Game.Entity    import Entity

from Core.Event 	  import Event
from Input          import KEY

