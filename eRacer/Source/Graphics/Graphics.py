from Core.Globals   import *
from Core.Config    import Config
from Window         import Window
from View           import View

class Graphics(Module):
  def __init__(self):
    Module.__init__(self)
    
    self.window   = Window("Test")
    self.graphics = cpp.GraphicsLayer.GetInstance()
    
    # get pointers
    
    game().hwnd = self.window.hwnd
    game().hwnd.disown()
    game().hinst = self.window.hinst
    game().hinst.disown()
    game().window = self.window
    
    
    
    self.graphics.Init(game().hwnd)
    
    self.d3d = self.graphics.GetDevice()
    self.d3d.disown()
    
    self.views = []
    self.scene  = None
    self.camera = None


  def Init(self):
    Module.Init(self)
    self.width  = self.graphics.width
    self.height = self.graphics.height

    
  def Tick(self, time):
    Module.Tick(self, time)
    self.window.Poll()
    
    
    fps = time.Fps()
    # if not game().ticks % 20: 
    #   self.window.SetTitle("eRacerX - %.2f FPS" % fps) # somehow this is really slow! go windows
       
    # alternately skip this frame so that physx gets more sim time
    # print int(fps / 60),  game().ticks % max(int(fps / 60), 1)
    if game().ticks % max(int(fps / 60), 1):
      # print 'skipping rendering'
      for view in self.views:
        view.Clear()
      self.views = []
      self.graphics.ClearStrings();
      return
    
    # if game().ticks != 1:
      # self.graphics.PostRender()
      
    
    self.graphics.PreRender()
    
    for view in self.views:
      view.Draw()
    self.views = []
    
    if CONSTS.DEBUG_FPS:
      self.graphics.WriteString("%.2f FPS" % fps, Config.DEBUG_FONT, 18, 10, 10)
  
    self.graphics.PostRender()
    
  
  def Quit(self):
    Module.Quit(self)
    self.graphics.Shutdown()
    # TODO close window
    