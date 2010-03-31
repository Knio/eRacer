#include "StringSprite.h"
#include "GraphicsLayer.h"

namespace Graphics {

StringSprite::StringSprite(float l, float t, float w, float h)
{
	GraphicsLayer* graphics = GraphicsLayer::GetInstance();
	// sprite = graphics->CreateSprite(0, 0, w);
	D3DXCreateSprite(graphics->m_pd3dDevice, &sprite);
	// ratio = h/(w*3/4.0f);
}

void StringSprite::Write(const char* text, const char* family, int size, const Vector3 &pos, const Vector3 &color){
	// //float wr = width/(float)GraphicsLayer::GetInstance()->width;
  //  //float hr = height/(float)GraphicsLayer::GetInstance()->height;
	// Point3 p = pos;
	// //p.x*=wr;
	// p.y*=ratio;
	// //Point3 p(790,10,0);
	strings.push_back(FontManager::instance.CreateStringRenderable(text,family,size,pos,color,sprite));
}

void StringSprite::Draw(IDirect3DDevice9* device) const{
	//sort(strings.begin(), strings.end());
	device->SetTransform(D3DTS_WORLDMATRIX(0), &IDENTITY);
	GraphicsLayer::GetInstance()->ResetViewport();

	sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_OBJECTSPACE);
	for (vector<StringRenderable>::const_iterator string = strings.begin();
		string != strings.end(); string++) {
		assert(NULL != string->m_pFont);
		RECT area = {string->m_uiScreenX,string->m_uiScreenY,0,0};
		string->m_pFont->DrawText( sprite, string->m_strTextBuffer.c_str(), -1, &area, DT_NOCLIP, string->m_color);

	}
	sprite->End();
}

void StringSprite::Clear(){
		strings.clear();
}

}