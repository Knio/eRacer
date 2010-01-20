from distutils.core import setup
import py2exe
 
setup(
  name='eRacer',
  version='1.0',
  packages=[
    'Source', 
    'Source/Graphics', 
    'Release',
    'Lib',
  ],
  console=['run.py']
)
