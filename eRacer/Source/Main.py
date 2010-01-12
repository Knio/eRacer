'''
eRacer game.
'''

from Game import Game, Logic, Entity

from Input    import Input
from Graphics import Graphics
from Physics  import Physics


class TestEntity(Entity):
  def tick(self, time):
    print 'Hi! %r' % time.delta

class eRacer(Game):
  def __init__(self):
    Game.__init__(self)
    self.addmodule(Input(self),    'input')
    self.addmodule(Logic(self),    'logic')
    self.addmodule(Physics(self),  'physics')
    self.addmodule(Graphics(self), 'graphics')
    
    self.logic.add(TestEntity())
    
    
