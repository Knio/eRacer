NAME = 'Cyclone'


# Track 2 by John

from Core.Globals import *

MUSIC = 'Adventure.mp3'

xScale = 0.5
yScale = 0.6
zScale = 0.5


TRACK = [ 
  cpp.Frame(Point3(-1375*xScale,   0,   0), +X+Y+Z),
  cpp.Frame(Point3(-1375*xScale,-125*yScale, 500*zScale), +X-Z),
  cpp.Frame(Point3( -750*xScale,-250*yScale, 500*zScale), -X+Y-Z),
  
  cpp.Frame(Point3(-1000*xScale,-500*yScale,   0), +Y),
  cpp.Frame(Point3( -750*xScale,   0,-500*zScale), +Z),
  cpp.Frame(Point3( -750*xScale, 400*yScale,   0), -Y),
  cpp.Frame(Point3( -600*xScale,   0, 300*zScale), -Z),
  cpp.Frame(Point3( -550*xScale,-300*yScale,   200), Y-Z),
  
  cpp.Frame(Point3( -375*xScale,-250*yScale, -550*zScale), -X+Z),
  cpp.Frame(Point3(-1125*xScale,-125*yScale,-1125*zScale), +X+Z),

  cpp.Frame(Point3(-1125*xScale,  50*yScale,  -50*zScale), +X),
  cpp.Frame(Point3( -875*xScale,  50*yScale,  -50*zScale), -Z),
  cpp.Frame(Point3( -625*xScale,  50*yScale,  -50*zScale), +Y),
  cpp.Frame(Point3( -375*xScale,  50*yScale,  -50*zScale), +Z),
  cpp.Frame(Point3( -125*xScale,  50*yScale,  -50*zScale), -Y),
  cpp.Frame(Point3(  125*xScale,  50*yScale,  -50*zScale), -Y),
  cpp.Frame(Point3(  375*xScale,  50*yScale,  -50*zScale), -Z),
  cpp.Frame(Point3(  625*xScale,  50*yScale,  -50*zScale), +Y),
  cpp.Frame(Point3(  875*xScale,  50*yScale,  -50*zScale), +Z),
  cpp.Frame(Point3( 1125*xScale,  50*yScale,  -50*zScale), -Y),

  cpp.Frame(Point3( 1375*xScale,  25*yScale,  -50*zScale), -X-Y-Z),
  cpp.Frame(Point3( 1375*xScale,-300*yScale, -250*zScale), -X+Y-Z),
  cpp.Frame(Point3(  500*xScale,-450*yScale, 0*zScale), +Y-Z),
  
  cpp.Frame(Point3(  600*xScale, 600*yScale, 400*zScale), +X-Z),
  cpp.Frame(Point3( 1375*xScale, 620*yScale, 1050*zScale), -X),
  cpp.Frame(Point3( 1375*xScale, 600*yScale,   0), -X+Y+Z),
  
  cpp.Frame(Point3(  375*xScale, 525*yScale, 175*zScale), +X+Y-Z),
  cpp.Frame(Point3(  375*xScale, 450*yScale, 550*zScale), +Y-Z),
  cpp.Frame(Point3( 1375*xScale, 375*yScale, 550*zScale), -X+Y-Z),
  cpp.Frame(Point3( 1375*xScale, 300*yScale,   0), -X+Y+Z),
  
  cpp.Frame(Point3(  375*xScale, 225*yScale,  50*zScale), +X+Y+Z),
  cpp.Frame(Point3(  375*xScale, 150*yScale, 550*zScale), +X+Y-Z),
  cpp.Frame(Point3( 1375*xScale,  75*yScale, 550*zScale), -X+Y-Z),
  cpp.Frame(Point3( 1375*xScale,   0,   0), -X+Y+Z),
  
  cpp.Frame(Point3( 1125*xScale,   -50,   0), +Y),
  cpp.Frame(Point3(  875*xScale,   -50,   0), -Z),
  cpp.Frame(Point3(  625*xScale,   -40,   0), -Y),
  cpp.Frame(Point3(  375*xScale,   -40,   0), +Z),
  cpp.Frame(Point3(  125*xScale,   -30,   0), +Y),
  cpp.Frame(Point3( -125*xScale,   -30,   0), +Y),
  cpp.Frame(Point3( -375*xScale,   -20,   0), -Z),
  cpp.Frame(Point3( -625*xScale,   -20,   0), -Y),
  cpp.Frame(Point3( -875*xScale,   -10,   0), +Z),
  cpp.Frame(Point3(-1125*xScale,   0,   0), +Z+X),
]

PROFILE = [
  cpp.TrackVertex(Point3(  25, -4,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25,  4,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25,  0,  0),  Y, 0.05, 0.01),
  cpp.TrackVertex(Point3(  15,  0,  0),  Y, 0.20, 0.01),  
  cpp.TrackVertex(Point3(   5,  0,  0),  Y, 0.40, 0.01),  
  cpp.TrackVertex(Point3(  -5,  0,  0),  Y, 0.60, 0.01),  
  cpp.TrackVertex(Point3( -15,  0,  0),  Y, 0.80, 0.01),  
  cpp.TrackVertex(Point3( -25,  0,  0),  Y, 0.95, 0.01),  
  cpp.TrackVertex(Point3( -25,  4,  0),  Y, 1.00, 0.01),
  cpp.TrackVertex(Point3( -25,  4,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3( -25, -4,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25, -4,  0),  Y, 1.00, 0.01),
  
]
