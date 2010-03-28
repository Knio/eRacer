from Core.Globals import *

SIZE = 400

TRACK = [
  cpp.Frame(Point3( -SIZE,  0,   00), Y),  
  cpp.Frame(Point3( -SIZE, 25, SIZE), normalized(Vector3( 1,1,-1))),
  cpp.Frame(Point3(   00,  50, SIZE), -Y),
  cpp.Frame(Point3(  -37,  50, -250), -Y),
  cpp.Frame(Point3(   00,-350, -200), -X),
  cpp.Frame(Point3(   00,-350, +200), +Y-Z),
  cpp.Frame(Point3(  +37,  50, +250), -Y),
  cpp.Frame(Point3(   00,  50,-SIZE), -Y),  
  cpp.Frame(Point3( SIZE,  25,-SIZE), normalized(Vector3(-1,0, 1))),
  cpp.Frame(Point3( SIZE,   0,   00), Y),  
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
  cpp.TrackVertex(Point3( -25, -4,  0),  Y, 0.00, 0.01),
  cpp.TrackVertex(Point3(  25, -4,  0),  Y, 1.00, 0.01),
]