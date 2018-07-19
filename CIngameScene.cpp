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
	for (int i = 0; i < 6; i++) {
		m_LHCircle[i] = new CHandCircle(CInputManager::HAND_SET::E_HAND_LEFT, i);
		m_RHCircle[i] = new CHandCircle(CInputManager::HAND_SET::E_HAND_RIGHT, i);
	}

	m_pPictures = new CPictures();
}

void CIngameScene::Update()
{
	m_pPictures->Update();

	for (int i = 0; i < 6; i++) {
		m_LHCircle[i]->Update();
		m_RHCircle[i]->Update();
	}
}

void CIngameScene::Render()
{

	m_pPictures->Render();

	for (int i = 0; i < 6; i++) {
		m_LHCircle[i]->Render();
		m_RHCircle[i]->Render();
	}

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

	for (int i = 0; i < 6; i++) {
		SAFE_DELETE(m_LHCircle[i]);
		SAFE_DELETE(m_RHCircle[i]);
	}
}
