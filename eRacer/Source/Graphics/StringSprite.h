#pragma once

#include "Renderable.h"
#include "FontManager.h"

namespace Graphics {

class StringSprite : public Renderable {
public:	
	StringSprite(float l, float t, float w, float h);
	void Write(const char* msg, const char* fontName, int size, const Vector3 &pos, const Vector3 &color);
	void Clear();

	virtual void Draw(IDirect3DDevice9*) const;
private:
	ID3DXSprite* sprite;
  
  float ratio;
	
  vector<StringRenderable> strings;
};

}