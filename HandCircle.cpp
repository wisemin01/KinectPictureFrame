#include "DXUT.h"
#include "HandCircle.h"


CHandCircle::CHandCircle(CInputManager::HAND_SET handSet)
	:m_handSet(handSet)
{
	if(m_handSet == CInputManager::HAND_SET::E_HAND_LEFT)
		m_circle = IMAGEMANAGER->GetTexture("LHCircle", "./Resource/LHCircle.png");
	else
		m_circle = IMAGEMANAGER->GetTexture("RHCircle", "./Resource/RHCircle.png");

	m_statusColor = D3DCOLOR_XRGB(255, 0, 0);
}


CHandCircle::~CHandCircle()
{
}

void CHandCircle::Update()
{
	HandState handState = INPUTMANAGER->GetHandState(0, m_handSet);

	switch (handState)
	{
	case HandState_Open:
		m_statusColor = D3DCOLOR_XRGB(255, 0, 0);
		break;
	case HandState_Closed:
		m_statusColor = D3DCOLOR_XRGB(0, 255, 0);
		break;
	case HandState_Lasso:
		m_statusColor = D3DCOLOR_XRGB(0, 0, 255);
		break;
	default:
		m_statusColor = D3DCOLOR_XRGB(0, 0, 0);
		break;
	}
}

void CHandCircle::Render()
{
	m_circle->CenterRender(INPUTMANAGER->GetHandPosition(0, m_handSet), m_statusColor);
}
