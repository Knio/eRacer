import os
import sys
import time
# paths to code
sys.path.append(os.path.join(sys.path[0], 'Source'))
sys.path.append(os.path.join(sys.path[0], 'Release'))
sys.path.append(os.path.join(sys.path[0], 'Source\\Swig'))
sys.path.append('.')

# set path to load fmod.dll
os.environ['PATH'] += ';Lib'


import run
import cProfile as profile
import pstats

profile.run('run.run()', 'profile.data')

stats = pstats.Stats('profile.data')
stats.sort_stats('cumulative') 
stats.print_stats()
