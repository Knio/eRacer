from Core.Globals import *
import ConfigParser

class Config(object):
  def __init__(self):
    self.cp = ConfigParser.ConfigParser()
    self.read()

  def read(self):
    self.cp = self.readFile('Config/eRacer.cnf')
    if game().debug:
      self.readFile('Config/eRacer_debug.cnf')
        
  def readFile(self, file):
    cp = ConfigParser.ConfigParser()
    cp.read(file)
    for k, v in cp.items('CONSTS'):
      k = k.upper()
      try:
        c = getattr(CONSTS, k)
        r = getattr(cp, 'get%s' % type(c).__name__)('CONSTS', k)
        setattr(CONSTS, k, r)
        print 'Set %s \t= %r' % (k,r)
      except:
        print 'Failed to set %s' % k
        import traceback
        traceback.print_exc()
    return cp
    
  def save(self):
    pass
    
    # with open('Config/eRacer.cnf', 'wb') as file:
    #   self.cp.write(file)
    #     