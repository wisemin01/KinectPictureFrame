#include "DXUT.h"
#include "CIngameScene.h"

#include "HandCircle.h"
#include "CPictures.h"

CIngameScene::CIngameScene()
{
	Init();
}


CIngameScene::~CIngameScene()
{
	Release();
}

void CIngameScene::Init()
{
	m_LHCircle = new CHandCircle(CInputManager::HAND_SET::E_HAND_LEFT);
	m_RHCircle = new CHandCircle(CInputManager::HAND_SET::E_HAND_RIGHT);

	m_pPictures = new CPictures();
}

void CIngameScene::Update()
{
	m_pPictures->Update();

	m_LHCircle->Update();
	m_RHCircle->Update();
}

void CIngameScene::Render()
{

	m_pPictures->Render();

	m_LHCircle->Render();
	m_RHCircle->Render();

#pragma region PRINTF HANDPOS
#if _DEBUG
	TEXTMANAGER->DrawString("LHAND", D3DXVECTOR2(0, 0));

	char str[100] = "X : %f";
	sprintf(str, "X : %f", INPUTMANAGER->GetHandPosition(0, CInputManager::HAND_SET::E_HAND_LEFT).x);
	TEXTMANAGER->DrawString(str, D3DXVECTOR2(0, 20));

	sprintf(str, "Y : %f", INPUTMANAGER->GetHandPosition(0, CInputManager::HAND_SET::E_HAND_LEFT).y);
	TEXTMANAGER->DrawString(str, D3DXVECTOR2(170, 20));

	TEXTMANAGER->DrawString("RHAND", D3DXVECTOR2(0, 40));

	sprintf(str, "X : %f", INPUTMANAGER->GetHandPosition(0, CInputManager::HAND_SET::E_HAND_RIGHT).x);
	TEXTMANAGER->DrawString(str, D3DXVECTOR2(0, 60));

	sprintf(str, "Y : %f", INPUTMANAGER->GetHandPosition(0, CInputManager::HAND_SET::E_HAND_RIGHT).y);
	TEXTMANAGER->DrawString(str, D3DXVECTOR2(170, 60));
#endif
#pragma endregion PRINTF HANDPOS
}

void CIngameScene::Release()
{
	SAFE_DELETE(m_pPictures);

	SAFE_DELETE(m_LHCircle);
	SAFE_DELETE(m_RHCircle);
}
