from Core.Globals import *


class Raceline:
  def __init__(self, points):
    self.waypoints = points # array of points 0 is the start node
    self.index = 0
    
  def UpdateWaypoint(self, carPos):
    #for each pair of points
    #compute segment between old point and the one after it
    #compute distance between carPosition and the segment.
    #if the distance gets larger, than we have found the closest piece already.
    if(len(self.waypoints) >= 2): #otherwise nothing to update
      smallestDist = 99999999
      oldPoint = self.Peek(0)
      nextPoint = self.Peek(1)
      segment = nextPoint - oldPoint
      #distnace between segment and carPosition
      distance = length(cross(nextPoint - oldPoint, oldPoint - carPos)) / length(nextPoint - oldPoint)
      #print distance
    
  
  """ return the waypoint that is offset points ahead. offset = 0 means the node ahead of this position"""
  def Peek(self, offset = 0):
    assert len(self.waypoints) > 0
    return self.waypoints[(self.index+offset) % len(self.waypoints)]