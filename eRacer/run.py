
def run():
  m = None
  try:
    from Core import Globals
    from Main import Main
    m = Main()
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
  run()    
