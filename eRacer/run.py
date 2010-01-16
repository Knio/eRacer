import sys
import time
# paths to code
sys.path.append('Release')
sys.path.append('Source')
sys.path.append('Source\\Swig')


from Main import Main


def run():
  m = None
  try:
    m = Main()
    m.Run()
  except:
    if m: m.Quit()
    import traceback
    import msvcrt
    traceback.print_exc()
    print 'Press any key to close'
    msvcrt.getch()
  
if __name__ == '__main__':
  run()    
