#pragma once

class CHandCircle
{
private:
	texture* m_circle;

	CInputManager::HAND_SET m_handSet;

	D3DCOLOR m_statusColor;

	INT m_iTargetNumber;
public:
	CHandCircle(CInputManager::HAND_SET handSet, int iTargetNum);
	~CHandCircle();

	void Update();
	void Render();
};

