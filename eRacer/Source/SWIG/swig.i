%module eRacer
%{
/* Includes the header in the wrapper code */
#include "..\Core\Time.h"
#include "..\Graphics\GraphicsLayer.h"
#include "..\Graphics\Window.h"
#include "..\Sound\SoundLayer.h"
#include "..\Physics\PhysicsLayer.h"
#include "..\Input\Keyboard.h"
%}

/* Parse the header file to generate wrappers */
%include "..\Core\Time.h"
%include "..\Graphics\GraphicsLayer.h"
%include "..\Graphics\Window.h"
%include "..\Sound\SoundLayer.h"
%include "..\Physics\PhysicsLayer.h"
%include "..\Input\Keyboard.h"