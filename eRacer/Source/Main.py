'''
eRacer game.
'''

from Game import Game, Entity

from Input    import Input
from Logic    import Logic
from Graphics import Graphics
from Physics  import Physics

import eRacer

class TestEntity(Entity):
  def tick(self, time):
    pass
    #print 'Hi! %10r %10r %10r' % (time.elapsed, time.delta, time.Fps())

class Main(Game):
  def __init__(self):
    Game.__init__(self)
    self.addmodule(Input(self),    'input')
    self.addmodule(Logic(self),    'logic')
    self.addmodule(Physics(self),  'physics')
    self.addmodule(Graphics(self), 'graphics')
    
    self.logic.add(TestEntity())
    
    from eRacer import SoundLayer
    s = SoundLayer.GetInstance()
    s.Init()
    print s.PlaySound2D("Resources/jaguar.wav")    
    
    self.keyboard = eRacer.Keyboard();
    self.keyboard.Init(self.graphics.window.hwnd, self.graphics.window.hinst)
  
  
  def tick(self, time):
    self.keyboard.Update()
    print self.keyboard.isKeyPressed(0x1e)
    Game.tick(self, time)
    