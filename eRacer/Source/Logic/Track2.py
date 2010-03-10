# Track 2 by John

from Core.Globals import *

TRACK = [ 
  cpp.Frame(Point3(-1375,   0,   0), +X+Y+Z),
  cpp.Frame(Point3(-1375,-125, 500), +X-Z),
  cpp.Frame(Point3( -750,-250, 500), -X+Y-Z),
  
  cpp.Frame(Point3(-1000,-500,   0), +Y),
  cpp.Frame(Point3( -750,   0,-500), +Z),
  cpp.Frame(Point3( -750, 500,   0), -Y),
  cpp.Frame(Point3( -750,   0, 500), -Z),
  cpp.Frame(Point3( -500,-500,   0), +Y),
  
  cpp.Frame(Point3( -375,-250, -550), -X+Y+Z),
  cpp.Frame(Point3(-1125,-125,-1125), +X+Y+Z),

  cpp.Frame(Point3(-1125,  50,  -50), -Y),
  cpp.Frame(Point3( -875,  50,  -50), -Z),
  cpp.Frame(Point3( -625,  50,  -50), +Y),
  cpp.Frame(Point3( -375,  50,  -50), +Z),
  cpp.Frame(Point3( -125,  50,  -50), -Y),
  cpp.Frame(Point3(  125,  50,  -50), -Y),
  cpp.Frame(Point3(  375,  50,  -50), -Z),
  cpp.Frame(Point3(  625,  50,  -50), +Y),
  cpp.Frame(Point3(  875,  50,  -50), +Z),
  cpp.Frame(Point3( 1125,  50,  -50), -Y),
  
  cpp.Frame(Point3( 1375,  25,  -50), -X-Y-Z),
  cpp.Frame(Point3( 1375,-300, -550), -X+Z),
  cpp.Frame(Point3(  375,-300, -550), +X+Y+Z),
  
  cpp.Frame(Point3(  375, 675, 550), +X+Y-Z),
  cpp.Frame(Point3( 1375, 675, 550), -X+Y-Z),
  cpp.Frame(Point3( 1375, 600,   0), -X+Y+Z),
  
  cpp.Frame(Point3(  375, 525, 175), +X+Y+Z),
  cpp.Frame(Point3(  375, 450, 550), +X+Y-Z),
  cpp.Frame(Point3( 1375, 375, 550), -X+Y-Z),
  cpp.Frame(Point3( 1375, 300,   0), -X+Y+Z),
  
  cpp.Frame(Point3(  375, 225,  50), +X+Y+Z),
  cpp.Frame(Point3(  375, 150, 550), +X+Y-Z),
  cpp.Frame(Point3( 1375,  75, 550), -X+Y-Z),
  cpp.Frame(Point3( 1375,   0,   0), -X+Y+Z),
  
  cpp.Frame(Point3( 1125,   0,   0), +Y),
  cpp.Frame(Point3(  875,   0,   0), -Z),
  cpp.Frame(Point3(  625,   0,   0), -Y),
  cpp.Frame(Point3(  375,   0,   0), +Z),
  cpp.Frame(Point3(  125,   0,   0), +Y),
  cpp.Frame(Point3( -125,   0,   0), +Y),
  cpp.Frame(Point3( -375,   0,   0), -Z),
  cpp.Frame(Point3( -625,   0,   0), -Y),
  cpp.Frame(Point3( -875,   0,   0), +Z),
  cpp.Frame(Point3(-1125,   0,   0), +Y),
]

PROFILE = [
  cpp.TrackVertex(Point3(  25, -1,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25,  2,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25,  0,  0),  Y, 0.05, 0.01),
  cpp.TrackVertex(Point3(  15,  0,  0),  Y, 0.20, 0.01),  
  cpp.TrackVertex(Point3(   5,  0,  0),  Y, 0.40, 0.01),  
  cpp.TrackVertex(Point3(  -5,  0,  0),  Y, 0.60, 0.01),  
  cpp.TrackVertex(Point3( -15,  0,  0),  Y, 0.80, 0.01),  
  cpp.TrackVertex(Point3( -25,  0,  0),  Y, 0.95, 0.01),  
  cpp.TrackVertex(Point3( -25,  2,  0),  Y, 1.00, 0.01),
  cpp.TrackVertex(Point3( -25, -1,  0),  Y, 0.00, 0.01),
  
  cpp.TrackVertex(Point3(  25, -1,  0),  Y, 1.00, 0.01),
  
]