#pragma once
class CHandCircle
{
private:
	texture* m_circle;

	CInputManager::HAND_SET m_handSet;

	D3DCOLOR m_statusColor;
public:
	CHandCircle(CInputManager::HAND_SET handSet);
	~CHandCircle();

	void Update();
	void Render();
};

