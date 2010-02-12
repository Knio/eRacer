#include "FontManager.h"
#include <cassert>


namespace Graphics {

	StringRenderable::StringRenderable(ID3DXSprite* targetSprite)
		: m_pTextSprite(targetSprite), m_pFont(NULL)
	{

	}


	StringRenderable::~StringRenderable()
	{
		m_pFont = NULL;
	}

	bool StringRenderable::operator<(const StringRenderable& s){
		return m_pFont < s.m_pFont;
	}

	void StringRenderable::Draw(IDirect3DDevice9*) const{
		assert(NULL != m_pFont);
		RECT area = {m_uiScreenX,m_uiScreenY,0,0};
		m_pFont->DrawText( m_pTextSprite, m_strTextBuffer.c_str(), -1, &area, DT_NOCLIP, m_color);

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
		for(map<FontDescription, ID3DXFont*>::const_iterator it =  m_fontCacheSimple.begin(); 
			it !=  m_fontCacheSimple.end(); ++it) {
			it->second->Release();
		}

		m_pd3dDevice = NULL;
		if ( NULL != m_pTextSprite )
		{
			m_pTextSprite->Release();
			m_pTextSprite = NULL;
		}
	}	

	void FontManager::WriteString(const char* msg, const char* fontFamily, int fontSize, const Vector3 &pos, const Vector3 &color)
	{
		FontDescription fontDesc(fontFamily, fontSize);
		map<FontDescription,ID3DXFont*>::iterator font;
		if ((font = m_fontCacheSimple.find(fontDesc)) == m_fontCacheSimple.end()) { //Cache Miss

			ID3DXFont* newFont = NULL;
			D3DXCreateFont( m_pd3dDevice, fontSize, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
									 OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                     fontFamily, &newFont );

			font = m_fontCacheSimple.insert(make_pair(fontDesc,newFont)).first;
		}
		

		//Buffer sring for rendering
		m_strList.push_back(StringRenderable(m_pTextSprite));
		StringRenderable& s = m_strList.back();

		s.m_pFont 			= font->second;
		s.m_strTextBuffer	= msg;
		s.m_color 			= D3DXCOLOR(color.x, color.y, color.z, 1.0f);
		s.m_uiScreenX = (unsigned int ) pos.x;
		s.m_uiScreenY= (unsigned int) pos.y;
	}

	void FontManager::Draw()
	{
		sort(m_strList.begin(), m_strList.end());

		m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
		for (vector<StringRenderable>::const_iterator i = m_strList.begin();
			i != m_strList.end(); i++) {
				i->Draw(m_pd3dDevice);
		}
		m_pTextSprite->End();
		
		m_strList.clear();
	}
};
