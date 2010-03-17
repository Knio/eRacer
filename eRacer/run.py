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
    from Logic.MenuState import MainMenuState
    from Logic.GameState import GameState
    
    m = Main()
    
    if profile:
      m.PushState(GameState('Track1'))
      m.Init()
      cProfile.runctx('m.Run(5000)', globals(), locals(), 'profile.data')
      stats = pstats.Stats('profile.data')
      stats.sort_stats('cumulative') 
      stats.print_stats(0.1)
      stats.print_callees(0.1)
    else:
      m.PushState(MainMenuState())
      m.Init()
      m.Run()
      
  except:
    if m: m.Quit()
    import traceback
    import msvcrt
    traceback.print_exc()
    print 'Press any key to close'
    msvcrt.getch()
  
if __name__ == '__main__':
  params = list(sys.argv[1:])
  print params
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
      raise Exception('Invaid command line argument "%s"' % p)
  
  
  # paths to code
  sys.path.append(os.path.join(sys.path[0], 'Source'))
  
  if debug: sys.path.append(os.path.join(sys.path[0], 'Debug'))
  else:     sys.path.append(os.path.join(sys.path[0], 'Release'))
  
  sys.path.append(os.path.join(sys.path[0], 'Source\\Swig'))
  sys.path.append(os.path.join(sys.path[0], 'Config'))
  
  # set path to load fmod.dll
  os.environ['PATH'] += ';Lib'
  
  run()