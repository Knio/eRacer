#pragma once
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <vector>
#include "../Core/Math.h"

namespace Graphics {

class StringRenderable
{
public:
	ID3DXFont* m_pFont;
	const char* m_strTextBuffer;
	RECT m_renderArea; //Position and area to render in
	D3DXCOLOR m_color;
	D3DXMATRIX m_scaling;

	StringRenderable();
	~StringRenderable();
};

class FontRenderable
{
public:
	ID3DXFont* m_pFont;
};


class FontManager
{
public:
	static const float FONT_SIZE;
	std::map<const char *, ID3DXFont*> m_fontCacheSimple;
	std::vector<StringRenderable> m_strList;	//List of strings to render

	LPDIRECT3DDEVICE9   m_pd3dDevice;
	ID3DXSprite*        m_pTextSprite;

protected:
	void SortStringList();

public:

	FontManager();
	~FontManager();

	void Init(LPDIRECT3DDEVICE9 device);

	//void WriteString(const char* msg, const char* fontName, float size, Vector3 pos, Vector3 color);
	void WriteString(const char* msg, const char* fontName, const float &size, const Vector3 &pos, const Vector3 &color);
	void WriteString(const char* msg, ID3DXSprite* font, Vector3 color);
	void GetFont(const char* fontName);
	void CreateFont(const char* fontName, int size, bool bold, bool italic); 
	void Draw();
	void Shutdown();
};

};
#endif