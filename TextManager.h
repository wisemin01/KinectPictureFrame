#pragma once
#include "singleton.h"
class CTextManager :
	public singleton<CTextManager>
{
private:
	LPD3DXFONT m_lpD3DXFont01;
	
	LPD3DXSPRITE m_lpD3DXSprite;
public:
	CTextManager();
	~CTextManager();

	void Init();
	void Release();

	void DrawString(const string & str, const D3DXVECTOR2 & pos);
};

#define TEXTMANAGER CTextManager::Instance()

