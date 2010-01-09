%module eRacer
%{
/* Includes the header in the wrapper code */
#include "..\Graphics\GraphicsLayer.h"
#include "..\Graphics\Window.h"
%}

/* Parse the header file to generate wrappers */
%include "..\Graphics\GraphicsLayer.h"
%include "..\Graphics\Window.h"
