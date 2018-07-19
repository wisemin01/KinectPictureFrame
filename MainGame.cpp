#include "DXUT.h"
#include "MainGame.h"

#include "HandCircle.h"

#include "CIngameScene.h"

CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
}

void CMainGame::Init()
{
	m_pCurScene = new CIngameScene();
}

void CMainGame::Update()
{
	INPUTMANAGER->InputUpdate();

	m_pCurScene->Update();
}

void CMainGame::Render()
{
	IMAGEMANAGER->Begin();

	m_pCurScene->Render();

	IMAGEMANAGER->End();
}

void CMainGame::Release()
{
	SAFE_DELETE(m_pCurScene);

	CImageManager::ReleaseInstance();
	CTextManager::ReleaseInstance();
	CInputManager::ReleaseInstance();
}
