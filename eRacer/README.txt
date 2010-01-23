eRacer

INSTALLATION


PREREQUISITES

The following must be installed:
  
  Visual Studio 2008
  Microsoft DirectX SDK (August 2009)
  NVIDIA PhysX SDK v2.8.1
  Boost 1.41.0
  Python 2.6

The following environment variables
must be set (your paths may vary):
  
  BOOST_DIR = F:\Program Files (x86)\Microsoft Visual Studio 9.0\boost_1_41_0
  DXSDK_DIR = F:\Program Files (x86)\Microsoft DirectX SDK (August 2009)
  PHYSX_DIR = F:\Program Files (x86)\NVIDIA Corporation\NVIDIA PhysX SDK\v2.8.1
  PYTHON_DIR= F:\Python26\


ADDITIONAL DEPENDANCIES

  If you wish to *edit* the code, the SWIG preprocessor is also
  required to regenerate the wrapper code:
    
  swig-1.3.40
  SWIG_DIR = P:\swigwin
  

BUILDING

Build the project eRacer/eRacer.sln in Release mode only. If you chose not to 
install SWIG, then exclude Header Files/SWIG/swig.i from the build.


RUNNING

To run the program, run eRacer/run.py with python.


CONTROLS

Spacebar - RAWR!



