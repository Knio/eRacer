#include "View.h"
#include "IO/IO.h"

namespace Graphics{
	Camera View::DEFAULT_CAMERA;
	const Viewport View::DEFAULT_VIEWPORT;

/*
void View::LoadSkyBox(const std::string& filename){
	assert(NULL != camera);
	
	const float SKYBOX_SIZE = 1000;

	SkyBox* skyBox = new SkyBox(*camera);

	//we might want to make IO use strings in future...
	IO::GetInstance()->LoadMesh(skyBox, filename.c_str());
	//skyBox_.SetTransform(CreateMatrix(SKYBOX_SIZE,SKYBOX_SIZE,SKYBOX_SIZE));
	viewDependantRenderables.push_back(skyBox);
}
*/
void View::AddRenderable(const Renderable* renderable){
	assert(NULL != renderable);
	viewDependantRenderables.push_back(renderable);
}

}