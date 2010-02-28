#pragma SWIG nowarn=454,503
%module(directors="1") eRacer
%{

/* Includes the header in the wrapper code */

// Core
#include <d3dx9math.h>
#include "..\Core\Math.h"
#include "..\Core\Event.h"
#include "..\Core\Time.h"
#include "..\Core\Consts.h"


//Game
#include "..\Game\Module.h"
#include "..\Game\Game.h"
#include "..\Game\State.h"


// IO
#include "..\IO\IO.h"

// Logic
#include "..\Logic\Track.h"

// Graphics
#include "..\Graphics\Renderable.h"
#include "..\Graphics\Camera.h"
#include "..\Graphics\Scene.h"
#include "..\Graphics\Mesh.h"
#include "..\Graphics\MeshNode.h"
#include "..\Graphics\StaticMeshNode.h"
#include "..\Graphics\MovingMeshNode.h"
#include "..\Graphics\GraphicsLayer.h"
#include "..\Graphics\Window.h"
#include "..\Graphics\Starfield.h"
#include "..\Graphics\SkyBox.h"
#include "..\Graphics\CoordinateCross.h"
#include "..\Graphics\Sprite.h"


// Sound
#include "..\Sound\SoundLayer.h"


// Physics
#include "..\Physics\PhysicsLayer.h"
#include "..\Physics\PhysicsObject.h"
#include "..\Physics\Box.h"
#include "..\Physics\Plane.h"
#include "..\Physics\TriMesh.h"

// Input
#include "..\Input\Device.h"
#include "..\Input\Mouse.h"
#include "..\Input\Keyboard.h"
#include "..\Input\Gamepad.h"
#include "..\Input\Input.h"



// Test
#include "..\Test\Test.h"

%}

/* *****************************************************************************

Parse the header file to generate wrappers 

These are the classes that get exposed to Python
If a class doesn't need to be exported to Python, it can be left out.
*/


// SWIG *******************************
%include "std_string.i"
%include "std_vector.i"


// Core *******************************
%include "..\Core\d3dx.h"
%include "..\Core\Math.h"
%include "..\Core\Time.h"
%include "..\Core\Consts.h"



// Game *******************************
%include "..\Game\State.h"


// Exception handling test. Currently only going to put director classes here
%include "exception.i"

%exception {
  try {
    $action
  } catch (Swig::DirectorException) { 
    SWIG_fail;
  } catch(...) {
    SWIG_exception(SWIG_RuntimeError,"Exception calling $fulldecl");
  }
}

%feature("director:except") {
    if ($error != NULL) {
        throw Swig::DirectorMethodException();
    }
}

// Core *******************************
%feature("director") Event;
%include "..\Core\Event.h"

// Game *******************************
%feature("director") Module;
%include "..\Game\Module.h"
%feature("director") Game;
%include "..\Game\Game.h"

// IO *********************************
%feature("director") IO;
%include "..\IO\IO.h"

%exception;


// Logic
%template(VectorTrackVertex) std::vector<TrackVertex>;
%include "..\Logic\Track.h"

// Graphics ***************************
// %include "..\Graphics\Renderable.h" // Something funny happens with this one. 

%include "..\Graphics\Camera.h"

%include "..\Graphics\Mesh.h"
%include "..\Graphics\MeshNode.h"

%include "..\Graphics\StaticMeshNode.h"
%include "..\Graphics\MovingMeshNode.h"
%include "..\Graphics\Scene.h"

%include "..\Graphics\GraphicsLayer.h"
%include "..\Graphics\Window.h"
%ignore Star;
%include "..\Graphics\Starfield.h"
%include "..\Graphics\CoordinateCross.h"
%include "..\Graphics\SkyBox.h"
%include "..\Graphics\Sprite.h"

// Sound ******************************
%include "..\Sound\SoundLayer.h"

// Physics ****************************
%include "..\Physics\PhysicsLayer.h"
%include "..\Physics\PhysicsObject.h"
%include "..\Physics\Box.h"
%include "..\Physics\Plane.h"
%include "..\Physics\TriMesh.h"

// Input ******************************
// Does python acctually need these?
// %include "..\Input\Device.h"
// %include "..\Input\Mouse.h"
// %include "..\Input\Keyboard.h"
// %include "..\Input\Gamepad.h"
%include "..\Input\Input.h"


// Test
%include "..\Test\Test.h"

