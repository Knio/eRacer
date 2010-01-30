'''
Game entity object interface
'''

class Entity(object):
  def __init__(self, game):
	self.game = game
    
  def Tick(self, time):
    pass
  
