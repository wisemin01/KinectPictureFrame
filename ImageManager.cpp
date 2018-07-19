#include "DXUT.h"

texture::texture(LPDIRECT3DTEXTURE9 lpTexture, D3DXIMAGE_INFO info)
	:m_lpTexture(lpTexture), m_info(info)
{}

texture::~texture()
{
	SAFE_RELEASE(m_lpTexture);
}

void texture::Render(const D3DXVECTOR2 & pos)
{
	if (m_lpTexture)
	{
		D3DXMATRIX mat;

		D3DXMatrixAffineTransformation2D(&mat, 1.f, nullptr, 0.f, &D3DXVECTOR2(pos.x, pos.y));
		IMAGEMANAGER->GetD3DXSprite()->SetTransform(&mat);
		IMAGEMANAGER->GetD3DXSprite()->Draw(m_lpTexture, nullptr, nullptr, nullptr, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void texture::CenterRender(const D3DXVECTOR2 & pos, const RECT & rect)
{
	if (m_lpTexture)
	{
		D3DXMATRIX mat;

		D3DXMatrixAffineTransformation2D(&mat, 1.f, nullptr, 0.f, &D3DXVECTOR2(pos.x, pos.y));
		IMAGEMANAGER->GetD3DXSprite()->SetTransform(&mat);
		IMAGEMANAGER->GetD3DXSprite()->Draw(m_lpTexture, &rect, &D3DXVECTOR3(80 / 2, 80 / 2, 0.f), nullptr, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void texture::CenterRender(const D3DXVECTOR2 & pos, const D3DCOLOR & color)
{
	if (m_lpTexture)
	{
		D3DXMATRIX mat;

		D3DXMatrixAffineTransformation2D(&mat, 1.f, nullptr, 0.f, &D3DXVECTOR2(pos.x - m_info.Width / 2, pos.y - m_info.Height / 2));
		IMAGEMANAGER->GetD3DXSprite()->SetTransform(&mat);
		IMAGEMANAGER->GetD3DXSprite()->Draw(m_lpTexture, nullptr, nullptr, nullptr, color);
	}
}


CImageManager::CImageManager()
{
	Init();
}


CImageManager::~CImageManager()
{
	Release();
}

void CImageManager::Init()
{
	D3DXCreateSprite(g_device, &m_lpD3DSprite);
}

void CImageManager::Release()
{
	for (auto iter : m_images)
	{
		SAFE_DELETE(iter.second);
	}

	m_images.clear();

	SAFE_RELEASE(m_lpD3DSprite);
}


void CImageManager::Begin()
{
	m_lpD3DSprite->Begin( D3DXSPRITE_ALPHABLEND );
}

void CImageManager::End()
{
	m_lpD3DSprite->End();
}

texture* CImageManager::GetTexture(const string & key, const string & path)
{
	auto find = m_images.find(key);

	if (find != m_images.end())
		return find->second;

	LPDIRECT3DTEXTURE9	lpTexture = nullptr;
	D3DXIMAGE_INFO		info;

	if (D3DXCreateTextureFromFileExA(g_device, path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, nullptr, &lpTexture) == S_OK)
	{
		return m_images.insert(make_pair(key, new texture(lpTexture, info))).first->second;
	}

	return nullptr;
}
