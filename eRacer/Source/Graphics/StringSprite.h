#pragma once

#include "Renderable.h"
#include "FontManager.h"

namespace Graphics {

class StringSprite : public Renderable {
public:	
	StringSprite();
	void Write(const char* msg, const char* fontName, int size, long x, long y, const Vector3 &color);
	void Clear();

	virtual void Draw(IDirect3DDevice9*) const;
private:
	ID3DXSprite* sprite;
  
  // float ratio;
	
  vector<StringRenderable> strings;
};

}