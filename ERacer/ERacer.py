import sys

# paths to import compiled C++ files
sys.path.append('Release')
sys.path.append('Debug')

# test is a C++ module
from test import test

# new C++ instance
t = test.Test()
t.hello("ERacer")
print t.n         # 0
t.n = 10          # set the value in C++
print t.getn()    # does C++ think it's 10?
