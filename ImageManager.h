#pragma once
#include "singleton.h"

struct texture
{
	LPDIRECT3DTEXTURE9		m_lpTexture;
	D3DXIMAGE_INFO			m_info;

	texture(LPDIRECT3DTEXTURE9 lpTexture, D3DXIMAGE_INFO info);
	~texture();

	void Render(const D3DXVECTOR2 & pos);
	void CenterRender(const D3DXVECTOR2 & pos, const RECT & rect);
	void CenterRender(const D3DXVECTOR2 & pos, const D3DCOLOR & color = D3DCOLOR_XRGB(255, 255, 255));
};

class CImageManager :
	public singleton<CImageManager>
{
private:
	map<string, texture*> m_images;
	
	LPD3DXSPRITE m_lpD3DSprite;

	void Init();
	void Release();
public:
	CImageManager();
	virtual ~CImageManager();

	void Begin();
	void End();
	
	texture* GetTexture(const string & key, const string & path = "");

	const LPD3DXSPRITE & GetD3DXSprite() { return m_lpD3DSprite; }
};

#define IMAGEMANAGER CImageManager::Instance()