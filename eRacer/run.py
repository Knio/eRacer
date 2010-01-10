import sys
import time

# paths to code
sys.path.append('Release')
sys.path.append('Source')
sys.path.append('Source/Swig')

from Graphics.Window import Window
from Graphics.Graphics import Graphics

w = Window("Test")
g = Graphics.GetGraphicsInstance()

g.Init(w.hwnd)

while 1:
  g.RenderFrame()
  if not w.Poll():
    break
  
  time.sleep(0.1)
  
g.ShutDown()
  
del w
del g
