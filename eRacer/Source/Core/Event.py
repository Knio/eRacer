import eRacer

class EventManager(eRacer.EventManager):
  def __init__(self):
    eRacer.EventManager.__init__(self)
    print 'Init!'
        
  def Register(self, func, obj):
    print 'Register', func, obj
        
  def Send(self, e):
    print 'TEST', e

