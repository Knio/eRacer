#include "StringSprite.h"
#include "GraphicsLayer.h"

namespace Graphics {

StringSprite::StringSprite(int left, int top, int width, int height){
	GraphicsLayer* graphics = GraphicsLayer::GetInstance();
  sprite = graphics->CreateSprite(left, top, width, height);
}

void StringSprite::Write(const char* text, const char* family, int size, const Vector3 &pos, const Vector3 &color){
	strings.push_back(FontManager::instance.CreateStringRenderable(text,family,size,pos,color,sprite));
}

void StringSprite::Draw(IDirect3DDevice9* device) const{
	//sort(strings.begin(), strings.end());

	sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_OBJECTSPACE);
	for (vector<StringRenderable>::const_iterator string = strings.begin();
		string != strings.end(); string++) {
			string->Draw(device);
	}
	sprite->End();
}

void StringSprite::Clear(){
		strings.clear();
}

}