%module eRacer
%{
/* Includes the header in the wrapper code */
#include "..\Core\Time.h"
#include "..\Graphics\GraphicsLayer.h"
#include "..\Graphics\Window.h"
%}

/* Parse the header file to generate wrappers */
%include "..\Core\Time.h"
%include "..\Graphics\GraphicsLayer.h"
%include "..\Graphics\Window.h"
