from ctypes import *
import eRacer

class Window(eRacer.Window):
  def __init__(self, title):
    proc = WINFUNCTYPE(c_long, c_int, c_uint, c_int, c_int)(self.MsgProc)
    eRacer.Window.__init__(self)
    self.Create(title)
  
  # UNUSED
  def MsgProc(self, hwnd, msg, wparam, lparam):
    print hwnd, msg, wparam, lparam        
    return windll.user32.DefWindowProcA(c_int(hwnd), c_int(message), c_int(wParam), c_int(lParam))