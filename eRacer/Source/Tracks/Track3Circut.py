NAME = 'Circut'

from Core.Globals import *

SIZE = 400

TRACK = [
  cpp.Frame(Point3( -SIZE,  0,   00), X+Y),
  cpp.Frame(Point3(     0,  0, SIZE), Y-Z),
  cpp.Frame(Point3(  SIZE,  0,   00), -X+Y),
  cpp.Frame(Point3(     0,  0,-SIZE), Y+Z)
]

PROFILE = [
  cpp.TrackVertex(Point3(  25, -3,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25,  3,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25,  0,  0),  Y, 0.05, 0.01),
  cpp.TrackVertex(Point3(  15,  0,  0),  Y, 0.20, 0.01),  
  cpp.TrackVertex(Point3(   5,  0,  0),  Y, 0.40, 0.01),  
  cpp.TrackVertex(Point3(  -5,  0,  0),  Y, 0.60, 0.01),  
  cpp.TrackVertex(Point3( -15,  0,  0),  Y, 0.80, 0.01),  
  cpp.TrackVertex(Point3( -25,  0,  0),  Y, 0.95, 0.01),  
  cpp.TrackVertex(Point3( -25,  3,  0),  Y, 1.00, 0.01),
  cpp.TrackVertex(Point3( -25, -3,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25, -3,  0),  Y, 1.00, 0.01),
]