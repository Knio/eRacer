#include "FontManager.h"



namespace Graphics {
	const float FontManager::FONT_SIZE = 128.0;

	StringRenderable::StringRenderable()
	{
	}


	StringRenderable::~StringRenderable()
	{
		m_strTextBuffer = NULL;
		m_pFont = NULL;
		//m_scaling = NULL;
	}

	FontManager::FontManager()
	{
		m_pd3dDevice = NULL;
		m_pTextSprite = NULL;
	}

	FontManager::~FontManager()
	{
	}

	void FontManager::Init(LPDIRECT3DDEVICE9 device)
	{
		m_pd3dDevice = device;
		D3DXCreateSprite( m_pd3dDevice, &m_pTextSprite );
	}

	void FontManager::Shutdown()
	{
		for( std::map<const char *, ID3DXFont*>::const_iterator it =  m_fontCacheSimple.begin(); it !=  m_fontCacheSimple.end(); ++it) {
			it->second->Release();
		}

		m_pd3dDevice = NULL;
		if ( NULL != m_pTextSprite )
		{
			m_pTextSprite->Release();
			m_pTextSprite = NULL;
		}
	}

	void FontManager::SortStringList()
	{
	}

	void FontManager::WriteString(const char* msg, const char* fontName, const float &size, const Vector3 &pos, const Vector3 &color)
	{
		//Check if font exists
		if (m_fontCacheSimple.count(fontName) == 0) { //Cache Miss

			ID3DXFont* newFont = NULL;
			//WCHAR wszFontName[128]; //convert to wide char
			//MultiByteToWideChar(CP_ACP, 0, fontName, -1, wszFontName,128); 
			D3DXCreateFont( m_pd3dDevice, FONT_SIZE, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
									 OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                     fontName, &newFont );

			m_fontCacheSimple[fontName] = newFont;
		}

		//Store the String for later
		RECT rc;
		rc.top = (LONG) pos.y;
		rc.left = (LONG) pos.x;
		rc.bottom = 0;
		rc.right = 0;

 		StringRenderable s;
		s.m_pFont = m_fontCacheSimple[fontName];
		s.m_strTextBuffer = msg;
		s.m_color = D3DXCOLOR(color.x, color.y, color.z, 1.0f);
		s.m_renderArea = rc;
		D3DXMatrixScaling(&s.m_scaling, size/FONT_SIZE, size/FONT_SIZE, 1.0f);
		D3DXMATRIX tmp;
		D3DXMatrixTranslation(&tmp, -pos.x*size/FONT_SIZE + pos.x, -pos.y*size/FONT_SIZE + pos.y, 0.0f);
		D3DXMatrixMultiply(&s.m_scaling, &s.m_scaling, &tmp);

		m_strList.push_back(s);
	
	}

	void FontManager::Draw()
	{
		//Sort Strings Here

		m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
		for (int i = 0; i < (int) m_strList.size(); i++) {
			//WCHAR wszmsg[128]; //convert to wide char
			//MultiByteToWideChar(CP_ACP, 0, m_strList[i].m_strTextBuffer, -1, wszmsg, 128);
			m_pTextSprite->SetTransform(&m_strList[i].m_scaling);
			m_strList[i].m_pFont->DrawText( m_pTextSprite, m_strList[i].m_strTextBuffer, -1, &m_strList[i].m_renderArea, DT_NOCLIP, m_strList[i].m_color );
		}
		m_pTextSprite->End();

		m_strList.clear();

	}

	void FontManager::GetFont(const char* fontName)
	{
		//Does nothing

		/*WCHAR wszFontName[128]; //convert to wide char
		//MultiByteToWideChar(CP_ACP, 0, fontName, -1, wszFontName,128); 

		ID3DXFont* newFont;

		D3DXCreateFont( m_pd3dDevice, 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
									 OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                     wszFontName, &newFont );

		m_fontCacheSimple[fontName] = newFont;*/
	}






};
