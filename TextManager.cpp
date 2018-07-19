#include "DXUT.h"

CTextManager::CTextManager()
{
	Init();
}


CTextManager::~CTextManager()
{
	Release();
}

void CTextManager::Init()
{
	m_lpD3DXSprite = IMAGEMANAGER->GetD3DXSprite();

	D3DXCreateFont(g_device, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"±¼¸²", &m_lpD3DXFont01);
}

void CTextManager::Release()
{
	SAFE_RELEASE(m_lpD3DXFont01);
}

void CTextManager::DrawString(const string & str, const D3DXVECTOR2 & pos)
{
	RECT tempRect = { 0 };

	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, pos.x, pos.y, 0.f);
	
	m_lpD3DXSprite->SetTransform(&mat);
	m_lpD3DXFont01->DrawTextA(m_lpD3DXSprite, str.c_str(), str.size(), &tempRect, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}
