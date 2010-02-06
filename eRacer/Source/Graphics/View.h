

#pragma once

#include "Camera.h"
#include "Scene.h"
#include <D3D9Types.h>

namespace Graphics{

typedef D3DVIEWPORT9 Viewport;

struct View {
	static const Camera DEFAULT_CAMERA;
	static const Viewport DEFAULT_VIEWPORT;

	View(const Scene* s, const Camera* c = &DEFAULT_CAMERA, const Viewport* v=&DEFAULT_VIEWPORT) : camera(c), scene(s), viewport(v) {};

	const Camera* camera;
	const Scene* scene;
	const Viewport* viewport;
};

}