import os
import sys
import time
# paths to code
sys.path.append(os.path.join(sys.path[0], 'Source'))
sys.path.append(os.path.join(sys.path[0], 'Debug'))
sys.path.append(os.path.join(sys.path[0], 'Source\\Swig'))
sys.path.append(os.path.join(sys.path[0], 'Config'))

# set path to load fmod.dll
os.environ['PATH'] += ';Lib'

# print sys.path
# print os.environ['PATH']

import run
if __name__ == '__main__':
  run.run()    
