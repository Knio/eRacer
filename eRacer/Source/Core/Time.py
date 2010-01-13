import eRacer

class Time(eRacer.Time):
  def __init__(self):
    eRacer.Time.__init__(self)
    self.buffer = [0]*10
    self.pos = 0
    
  def Tick(self):
    r = eRacer.Time.Tick(self)
    self.buffer[self.pos] = self.elapsed;
    self.pos = (self.pos + 1) % len(self.buffer)
    return r
    
  def Fps(self):
    return self.RESOLUTION \
     * float(len(self.buffer)-1) \
     / (self.buffer[(self.pos-1) % 10] - self.buffer[self.pos])