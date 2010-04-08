from Core.Globals import *
import ConfigParser

class Config(object):
  CONSTANTS_FILE  = 'Config/Constants.cnf'
  DEBUG_FILE      = 'Config/Debug.cnf'
  GLOBAL_FILE     = 'Config/Settings.cnf'
  USER_FILE       = 'User/Settings.cnf'
  
  
  def __init__(self):
    self.read()

  def read(self):
    self.constants = self.readFile([self.CONSTANTS_FILE, game().debug and self.DEBUG_FILE])
    self.parseConstants(self.constants)
    self.settings = self.readFile([self.GLOBAL_FILE, self.USER_FILE])
    self.user = self.readFile(self.USER_FILE)
    
        
  def readFile(self, file):
    cp = ConfigParser.ConfigParser()
    cp.read(file)
    return cp

  def parseConstants(self, cp):
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
    
    
  def save(self):
    with open(self.USER_FILE, 'wb') as file:
      self.user.write(file)
      
  def set_setting(self, key, value, section='GENERAL'):
    if not self.user.has_section(section):
      self.user.add_section(section)
    self.user.set(section, key, value)
    self.settings.set(section, key, value)
    
  def get_setting(self, key, section='GENERAL'):
    return self.settings.get(section, key)
    
         