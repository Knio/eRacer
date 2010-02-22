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


// SWIG
%include "std_string.i"
%include "std_vector.i"


// Core

%rename(m11) D3DXMATRIX::_11;

%include "..\Core\d3dx.h"
%include "..\Core\Math.h"
%feature("director") Event;
%include "..\Core\Event.h"
%include "..\Core\Time.h"
%include "..\Core\Consts.h"



// Game
%feature("director") Module;
%include "..\Game\Module.h"
%feature("director") Game;
%include "..\Game\Game.h"
%include "..\Game\State.h"


// IO
%feature("director") IO;
%include "..\IO\IO.h"



// Graphics
// %include "..\Graphics\Renderable.h"

%include "..\Graphics\Camera.h"

// Python doesn't need these declarations
// %include "..\Graphics\AxisAlignedBoundingBox.h"
// %include "..\Graphics\Spatial.h"
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

// Sound
%include "..\Sound\SoundLayer.h"


// Physics
%include "..\Physics\PhysicsLayer.h"
%include "..\Physics\PhysicsObject.h"
%include "..\Physics\Box.h"
%include "..\Physics\Plane.h"
%include "..\Physics\TriMesh.h"


// Input
%include "..\Input\Device.h"
%include "..\Input\Mouse.h"
%include "..\Input\Keyboard.h"
%include "..\Input\Gamepad.h"
%include "..\Input\Input.h"



// Test
%include "..\Test\Test.h"
