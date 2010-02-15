from Core.Globals   import *
from Window         import Window
from View           import View

class Graphics(Module):
  def __init__(self, game):
    Module.__init__(self, game)
    
    self.window   = Window("Test")
    self.graphics = eRacer.GraphicsLayer.GetInstance()
    
    # get pointers
    
    self.hwnd = self.window.hwnd
    self.hwnd.disown()
    self.hinst = self.window.hinst
    self.hinst.disown()
    
    self.graphics.Init(self.hwnd)
    
    self.d3d = self.graphics.GetDevice()
    self.d3d.disown()
    
    self.views = []
    self.scene  = None
    self.camera = None


  def Init(self):
    Module.Init(self)
    
  def Tick(self, time):
    Module.Tick(self, time)
    self.window.Poll()
    
    fps = time.Fps()
    self.window.SetTitle("eRacerX - %.2f FPS" % fps)
        
    if 1.0/fps > CONSTS.PHYS_MAX_TIMESTEP:
      # we are lagging
      # alternately skip this frame so that physx gets more sim time
      if game().ticks % 2: 
        print 'skipping rendering'
        self.views = []
        return
    
    self.graphics.PreRender();
        
    for view in self.views:
      view.Draw()
    self.views = []
    
    self.graphics.PostRender();
    
  
  def Quit(self):
    Module.Quit(self)
    self.graphics.Shutdown()
    # TODO close window
    