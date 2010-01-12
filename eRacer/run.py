import sys
import time
# paths to code
sys.path.append('Release')
sys.path.append('Source')
sys.path.append('Source/Swig')

from Main import eRacer


def run():
  eRacer().run()
  
if __name__ == '__main__':
  run()
