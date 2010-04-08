from Core.Globals import *
import ConfigParser

class Config(object):
  def __init__(self):
    self.changes = ConfigParser.ConfigParser()
    self.read()

  def read(self):
    self.current = self.readFile(['Config/Global.cnf','Config/User.cnf'])
    
    if game().debug:
      self.readFile('Config/Debug.cnf')
        
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
    with open('Config/User.cnf', 'wb') as file:
      self.changes.write(file)
      
  def set_setting(self, key, value):
    if not self.changes.has_section('SETTINGS'):
      self.changes.add_section('SETTINGS')
    self.changes.set('SETTINGS', key, value)
    self.current.set('SETTINGS', key, value)
    
  def get_setting(self, key):
    return self.current.get('SETTINGS', key)
    
         