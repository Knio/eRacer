import os
import sys
import math

import eRacer as cpp

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
  return "(%8.2g %8.2g %8.2g)" % (self.x, self.y, self.z)

cpp.D3DXVECTOR3.__iadd__ = VECTOR3__iadd__
cpp.D3DXVECTOR3.__repr__ = VECTOR3__repr__


Vector3   = cpp.D3DXVECTOR3
Vector4   = cpp.D3DXVECTOR4
Point3    = cpp.D3DXVECTOR3
Point4    = cpp.D3DXVECTOR4

Matrix    = cpp.CreateMatrix

ORIGIN    = cpp.cvar.ORIGIN
IDENTITY  = cpp.cvar.IDENTITY
X         = cpp.cvar.X
Y         = cpp.cvar.Y
Z         = cpp.cvar.Z
RED       = cpp.cvar.RED
BLUE      = cpp.cvar.BLUE
GREEN     = cpp.cvar.GREEN
WHITE     = cpp.cvar.WHITE

length    = cpp.abs
dot       = cpp.dot
cross     = cpp.cross
normalize = cpp.normalize
normalized= cpp.normalized
normal    = cpp.normalized
project   = cpp.project
projectOnto=cpp.projectOnto
mul1      = cpp.mul1
mul0      = cpp.mul0

MeshNode = cpp.MeshNode



PI    = math.pi
ZERO  = 1e-5

def clamp(x, _min, _max):
  return min(max(x, _min), _max)

CONSTS    = cpp.Constants().g_Constants

from Game.Module  	import Module
from Game.Entity    import Entity
from Logic.Prop      import Prop

from Core.Event 	  import Event
from Input          import KEY

