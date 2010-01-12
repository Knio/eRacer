import run
import cProfile as profile
import pstats

profile.run('run.run()', 'profile.data')

stats = pstats.Stats('profile.data')
stats.sort_stats('cumulative') 
stats.print_stats()
