from Core.Globals import *


class Raceline:
  def __init__(self, points):
    self.waypoints = points # array of points 0 is the start node
    self.index = 0
    
  """Sets the next node for the car to follow. The node does not update if the player is
    driving in the wrong direction"""  
  def UpdateWaypoint(self, carPos):
    #for each pair of points
    #compute segment between old point and the one after it
    #compute distance between carPosition and the segment.
    #if the distance gets larger, than we have found the closest piece already.
    if(len(self.waypoints) >= 2): #otherwise nothing to update
      smallestDist = 99999999
      for i in range(-1, len(self.waypoints)-1):
        oldPoint = self.Peek(i)
        nextPoint = self.Peek(i+1)
        segment = nextPoint - oldPoint
        #distnace between segment and carPosition
        distance = length(cross(nextPoint - oldPoint, oldPoint - carPos)) / length(nextPoint - oldPoint)
        oldi = (self.index+i) % len(self.waypoints)
        #print "dist from segment", oldi, "to", (oldi+1)%len(self.waypoints), "is:", distance
        if distance < smallestDist:
          smallestDist = distance
        else:
          self.index = oldi
          break
  
  """ return the waypoint that is offset points ahead. offset = 0 means the node we wish to head towards,
      -1 means previous node"""
  def Peek(self, offset = 0):
    assert len(self.waypoints) > 0
    return self.waypoints[(self.index+offset) % len(self.waypoints)]