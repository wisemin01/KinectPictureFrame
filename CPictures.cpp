#include "DXUT.h"
#include "CPictures.h"

#include "CImageFileLoader.h"

CPictures::CPictures()
	: m_iCurrentImage	(0),
	  m_iNextImage		(0),
	  m_iMaxImageCount	(0),
	  m_bChangeWay		(0),
	  m_iChangeXPos		(0),
	  m_iCurrentState	(E_IDLE),
	  m_pTextures		(nullptr),
	  m_dwLastOperate	(timeGetTime())
{
	Init();
}


CPictures::~CPictures()
{
	Release();
}

void CPictures::Init()
{
	// ���ϴ� ������ �̹����� ��� �о���� ��ü
	CImageFileLoader loader;
	// vector<texture*>* ���·� ��ȯ
	m_pTextures = loader.LoadImages("ImageList");

	// ������ ������ �˾ƿ�
	m_iMaxImageCount = m_pTextures->size() - 1;

	for (int i = 0; i < 6; i++)
	{
		m_handState[i] = HandState_Unknown;
		m_vHandPosition[i] = D3DXVECTOR2(0, 0);
		m_vFirstLockPosition[i] = D3DXVECTOR2(0, 0);
	}
}

void CPictures::Update()
{
	for (int i = 0; i < 6; i++)
	{
		m_handState[i] = INPUTMANAGER->GetHandState(i, CInputManager::E_HAND_RIGHT);
		m_vHandPosition[i] = INPUTMANAGER->GetHandPosition(i, CInputManager::E_HAND_RIGHT);
	}

	// �ڵ����� ���� ���� ( ���� : ������ )
	UpdateFrameTimer(30000);
	// ����� ���¿� ���� ��ü�� ������Ʈ
	UpdateStateMachine();
}

void CPictures::Render()
{
	RenderStateMachine();

	// DEBUG TOOL : 
	TEXTMANAGER->DrawString(to_string(m_dwLastOperate) + " : " + to_string(timeGetTime()), D3DXVECTOR2(300, 300));
}

void CPictures::Release()
{
	m_pTextures->clear();

	SAFE_DELETE(m_pTextures);
}

// ������ �ð����� ������ �ڵ����� ����
void CPictures::UpdateFrameTimer(DWORD dwDelay)
{
	if (m_iCurrentState == E_IDLE && m_dwLastOperate + dwDelay <= timeGetTime())
	{
		m_iCurrentState = E_CHANGE_FRAME;
		m_bChangeWay = E_RIGHT;

		// SET NEXT IMAGE FRAME : 
		if (m_iCurrentImage + 1 > m_iMaxImageCount)
			m_iNextImage = 0;
		else
			m_iNextImage = m_iCurrentImage + 1;

		m_iChangeXPos = 0;
	}
}

// �̹��� ��ȯ�� ���� �Է� ���� ����
void CPictures::InputProcessing()
{
	if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
	{
		m_iCurrentState = E_CHANGE_FRAME; m_bChangeWay = E_RIGHT;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)
	{
		m_iCurrentState = E_CHANGE_FRAME; m_bChangeWay = E_LEFT;
	}

	for (int i = 0; i < 6; i++)
	{
		static HandState prevHandState[6];

		prevHandState[i] = m_handState[i];

		if (prevHandState[i] == HandState_Open && m_handState[i] == HandState_Closed) {
			m_vFirstLockPosition[i] = m_vHandPosition[i];
		}

		if (prevHandState[i] == HandState_Closed && m_handState[i] == HandState_Open) {
			int Length = pow(m_vHandPosition[i].x - m_vFirstLockPosition[i].x, 2)
				+ pow(m_vHandPosition[i].y - m_vFirstLockPosition[i].y, 2);

			Length = sqrt(Length);

			if (Length > 100)
			{
				m_iCurrentState = E_CHANGE_FRAME;

				if (m_vHandPosition[i].x > m_vFirstLockPosition[i].x)
					// ����
					m_bChangeWay = E_LEFT;
				else
					// ������
					m_bChangeWay = E_RIGHT;
			}
		}
	}
}

// ����� ���¿� ���� ��ü�� ������Ʈ
void CPictures::UpdateStateMachine()
{
#pragma region IDLE
	if (m_iCurrentState == E_IDLE) {
		// INPUT : 
		InputProcessing();

		// SET NEXT IMAGE FRAME : 
		if (m_iCurrentState == E_CHANGE_FRAME)
		{
			if (m_bChangeWay == E_RIGHT)
			{
				if (m_iCurrentImage + 1 > m_iMaxImageCount)
					m_iNextImage = 0;
				else
					m_iNextImage = m_iCurrentImage + 1;
			}
			else if (m_bChangeWay == E_LEFT)
			{
				if (m_iCurrentImage - 1 < 0)
					m_iNextImage = m_iMaxImageCount;
				else
					m_iNextImage = m_iCurrentImage - 1;
			}

			m_iChangeXPos = 0;
		}
	}
#pragma endregion
#pragma region CHANGE_FRAME
	else if (m_iCurrentState == E_CHANGE_FRAME)
	{
		if (m_iChangeXPos >= WINSIZEX)
			m_iCurrentState = E_IDLE;

		// MOVE IMAGE : 
		lerp<int>(&m_iChangeXPos, m_iChangeXPos, WINSIZEX + 20, 0.07f);

		// SET NOW IMAGE FRAME : 
		if (m_iCurrentState == E_IDLE) {
			m_iCurrentImage = m_iNextImage;
			m_dwLastOperate = timeGetTime();
		}
	}
#pragma endregion
}

// ����� ���¿� ���� ��ü�� ����
void CPictures::RenderStateMachine()
{
#pragma region IDLE_RENDER
	if (m_iCurrentState == E_IDLE) {
		(*m_pTextures)[m_iCurrentImage]->Render(D3DXVECTOR2(0, 0));
	}
#pragma endregion
#pragma region CHANGE_FRAME
	if (m_iCurrentState == E_CHANGE_FRAME) {
		if (m_bChangeWay == E_RIGHT) {
			(*m_pTextures)[m_iCurrentImage]->Render(D3DXVECTOR2(-m_iChangeXPos, 0));
			(*m_pTextures)[m_iNextImage]->Render(D3DXVECTOR2(WINSIZEX - m_iChangeXPos, 0));
		}
		else if (m_bChangeWay == E_LEFT) {
			(*m_pTextures)[m_iCurrentImage]->Render(D3DXVECTOR2(m_iChangeXPos, 0));
			(*m_pTextures)[m_iNextImage]->Render(D3DXVECTOR2(-WINSIZEX + m_iChangeXPos, 0));
		}
	}
#pragma endregion
}
