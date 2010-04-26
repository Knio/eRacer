import os
import sys
import time

debug   = True
profile = False

def run():
  m = None
  try:
    from Core import Globals
    from Main import Main
    from Logic.MenuState      import MainMenuState
    from Logic.GameState      import GameState
    from Logic.GameSettings  import GameSettings
        
    if debug:
      import gc
      gc.set_debug(gc.DEBUG_COLLECTABLE | gc.DEBUG_UNCOLLECTABLE | gc.DEBUG_INSTANCES | gc.DEBUG_OBJECTS)
    
    m = Main(debug)
    
    if profile:
      m.Init()
      settings = GameSettings()
      m.PushState(GameState(settings))
      cProfile.runctx('m.Run(3000)', globals(), locals(), 'profile.data')
      stats = pstats.Stats('profile.data')
      stats.sort_stats('cumulative') 
      stats.print_stats(0.1)
      stats.print_callees(0.1)
      
    else:
      m.Init()
      m.PushState(MainMenuState())
      m.Run()
      
  except:
    import traceback
    import msvcrt
    traceback.print_exc()
    if m: m.Quit()
    print 'Press any key to close'
    msvcrt.getch()
  
if __name__ == '__main__':
  params = list(sys.argv[1:])
  while params:
    p = params.pop(0)
    if p == '--debug':
      debug = True
    elif p == '--release':
      debug = False  
    elif p == '--profile':
      profile = True
      import cProfile
      import pstats
    else:
      print 'Invalid command line argument "%s"' % p
      print 'Usage: run.py [--debug | --release | --profile]'
      sys.exit()  
  
  # paths to code
  sys.path.append(os.path.join(sys.path[0], 'Source'))
  
  if debug: sys.path.append(os.path.join(sys.path[0], 'Debug'))
  else:     sys.path.append(os.path.join(sys.path[0], 'Release'))
  
  sys.path.append(os.path.join(sys.path[0], 'Source\\Swig'))
  sys.path.append(os.path.join(sys.path[0], 'Config'))
  
  # set path to load fmod.dll
  os.environ['PATH'] += ';Lib'
  
  run()
