#pragma SWIG nowarn=454
%module(directors="1") eRacer
%{

/* Includes the header in the wrapper code */
#include "..\Core\Event.h"
#include "..\Core\Time.h"

#include "..\Graphics\Camera.h"
#include "..\Graphics\Scene.h"
#include "..\Graphics\StaticGeometry.h"
#include "..\Graphics\MovingGeometry.h"
#include "..\Graphics\GraphicsLayer.h"
#include "..\Graphics\Window.h"

#include "..\Sound\SoundLayer.h"

#include "..\Physics\PhysicsLayer.h"

#include "..\Input\Keyboard.h"

%}

/* Parse the header file to generate wrappers */

%feature("director") Event;
%include "..\Core\Event.h"
%include "..\Core\Time.h"

%include "..\Graphics\Camera.h"
%include "..\Graphics\StaticGeometry.h"
%include "..\Graphics\MovingGeometry.h"
%include "..\Graphics\Scene.h"
%include "..\Graphics\StaticGeometry.h"
%include "..\Graphics\MovingGeometry.h"
%include "..\Graphics\GraphicsLayer.h"
%include "..\Graphics\Window.h"

%include "..\Sound\SoundLayer.h"

%include "..\Physics\PhysicsLayer.h"

%include "..\Input\Keyboard.h"

