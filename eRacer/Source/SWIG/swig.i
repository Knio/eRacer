#pragma SWIG nowarn=454
%module(directors="1") eRacer
%{

/* Includes the header in the wrapper code */
#include "..\Core\Event.h"
#include "..\Core\Time.h"

#include "..\Graphics\Renderable.h"
#include "..\Graphics\TriMesh.h"
#include "..\Graphics\GraphicsLayer.h"
#include "..\Graphics\Window.h"

#include "..\Sound\SoundLayer.h"

#include "..\Physics\PhysicsLayer.h"

#include "..\Input\Keyboard.h"

%}

/* Parse the header file to generate wrappers */

%feature("director") EventManager;
%include "..\Core\Event.h"
%include "..\Core\Time.h"

%include "..\Graphics\Renderable.h"
%include "..\Graphics\TriMesh.h"
%include "..\Graphics\GraphicsLayer.h"
%include "..\Graphics\Window.h"

%include "..\Sound\SoundLayer.h"

%include "..\Physics\PhysicsLayer.h"

%include "..\Input\Keyboard.h"

