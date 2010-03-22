#pragma once
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "Renderable.h"

#include "../Core/Math.h"

using namespace std;

namespace Graphics {

typedef pair<string, int> FontDescription;

class StringRenderable
{
public:
	ID3DXFont* m_pFont;
	string m_strTextBuffer;
	unsigned int m_uiScreenX;
	unsigned int m_uiScreenY;
	D3DXCOLOR m_color;
	ID3DXSprite* m_pTextSprite;

	StringRenderable(ID3DXSprite* targetSprite);
	~StringRenderable();
	/**
	 * @brief Comparison operator for sorting - sorts by the address of the font used
	 */
	bool operator<(const StringRenderable& s) const;

};



class FontManager
{
public:
	const static char* CUSTOM_FONTS[1];

	static FontManager instance;

	FontManager();
	~FontManager();

	StringRenderable CreateStringRenderable(	const char* msg, 
																	const char* fontFamily, 
																	int fontSize, 
																	const Vector3 &pos, 
																	const Vector3 &color,
																	ID3DXSprite* sprite);
	void Shutdown();
	
private:
	map<FontDescription, ID3DXFont*> cache;
};

};
#endif