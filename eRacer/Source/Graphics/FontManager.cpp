#include "FontManager.h"
#include <cassert>
#include <stdexcept>
#include "GraphicsLayer.h"

using namespace std;


namespace Graphics {

	StringRenderable::StringRenderable(ID3DXSprite* targetSprite)
		: m_pTextSprite(targetSprite), m_pFont(NULL)
	{

	}


	StringRenderable::~StringRenderable()
	{
		m_pFont = NULL;
	}

	bool StringRenderable::operator<(const StringRenderable& s) const{
		return m_pFont < s.m_pFont;
	}



	const char* FontManager::CUSTOM_FONTS[1] = {"Resources\\Fonts\\Sony_Sketch_EF.ttf"};

	FontManager FontManager::instance;


	FontManager::FontManager()
	{
		int nFontsLoaded = AddFontResource("Resources\\Fonts\\Sony_Sketch_EF.ttf");
		cout << "Loaded "<< nFontsLoaded << " custom fonts."<< endl;
	}

	FontManager::~FontManager()
	{
	}

	void FontManager::Shutdown()
	{
		for(map<FontDescription, ID3DXFont*>::const_iterator mapping =  cache.begin(); 
			mapping !=  cache.end(); mapping++) {
			mapping->second->Release();
		}
	}	

	StringRenderable FontManager::CreateStringRenderable(	const char* msg, 
															const char* fontFamily, 
															int fontSize, 
															long x,
															long y, 
															const Vector3 &color,
															ID3DXSprite* sprite)
	{
		FontDescription fontDesc(fontFamily, fontSize);
		map<FontDescription,ID3DXFont*>::iterator font;
		if ((font = cache.find(fontDesc)) == cache.end()) { //Cache Miss

			ID3DXFont* newFont = NULL;
			HRESULT hr = D3DXCreateFont( 	GraphicsLayer::GetInstance().GetDevice(), 
																		fontSize, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
									 									OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                    fontFamily, &newFont );
			assert(D3DERR_INVALIDCALL != hr);
			assert(D3DXERR_INVALIDDATA != hr);
			
			if(E_OUTOFMEMORY == hr){
				cout << "Ran out of memory for custom font" << endl;
				HRESULT hr = D3DXCreateFont( 	GraphicsLayer::GetInstance().GetDevice(), 
																			fontSize, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
							 												OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                             					"Verdana", &newFont );
			}

			font = cache.insert(make_pair(fontDesc,newFont)).first;
		}
		

		//Buffer sring for rendering
		StringRenderable s(sprite);

		s.m_pFont 			= font->second;
		s.m_strTextBuffer	= msg;
		s.m_color 			= D3DXCOLOR(color.x, color.y, color.z, 1.0f);
		s.m_uiScreenX = x;
		s.m_uiScreenY = y;
		
		return s;
	}

};
