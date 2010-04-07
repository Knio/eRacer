from Core.Globals import *
import ConfigParser

class Config(object):
  def __init__(self):
    self.cp = ConfigParser.ConfigParser()
    self.read()

  def read(self):
    self.readFile('Config/eRacer.cnf')
    if game().debug:
      self.readFile('Config/eRacer_debug.cnf')
        
  def readFile(self, file):
    self.cp.read(file)
    for k, v in self.cp.items('CONSTS'):
      k = k.upper()
      try:
        c = getattr(CONSTS, k)
        r = getattr(self.cp, 'get%s' % type(c).__name__)('CONSTS', k)
        setattr(CONSTS, k, r)
        print 'Set %s \t= %r' % (k,r)
      except:
        print 'Failed to set %s' % k
        import traceback
        traceback.print_exc()
        
        