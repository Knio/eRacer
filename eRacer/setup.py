from distutils.core import setup
import py2exe
import glob
import sys
import os
 
# paths to code
sys.path.append(os.path.join(sys.path[0], 'Release'))
sys.path.append(os.path.join(sys.path[0], 'Source'))
sys.path.append(os.path.join(sys.path[0], 'Source\\Swig'))
sys.path.append(os.path.join(sys.path[0], 'Lib'))

print sys.path

setup(
  name='eRacer',
  version='1.0',
  console=['run-release.py'],
  #zipfile='eRacer.data',
  options={
    'py2exe': {
      'includes':[
        'ctypes',
        'zlib',
        'eRacer',
      ],
      'packages':[
        #'Source'
      ],
      'bundle_files': 3,
      'compressed': True,
      'dist_dir':'T:/dist'
    }
  },
  data_files=[
  ('Config', glob.glob('Config/*')),
  ('Shaders', glob.glob('Shaders/*')),
  ] + [(i, glob.glob(i+'/*')) for i in glob.glob('Resources/*')]
)
