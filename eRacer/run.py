import sys
import time
import testdoxy #just to test doxygen, not used at all
# paths to code
sys.path.append('Release')
sys.path.append('Source')
sys.path.append('Source/Swig')

from Graphics.Window import Window
from Graphics.Graphics import Graphics
w = Window("Test")
g = Graphics.GetGraphicsInstance()
h = w.hwnd
h.disown()
g.Init(h)

while w.Poll():
  g.RenderFrame()
  time.sleep(0.1)
  
g.Shutdown()
  
del w
del g
