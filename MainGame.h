#pragma once

class CScene;

class CMainGame
{
private:

	CScene * m_pCurScene;

public:
	CMainGame();
	~CMainGame();

	void Init();
	void Release();
	void Update();
	void Render();
};

