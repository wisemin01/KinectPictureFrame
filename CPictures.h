#pragma once
class CPictures
{
private:
	enum PICTURE_FRAME_STATE
	{
		E_IDLE,
		E_CHANGE_FRAME
	};
	enum CHANGE_WAY
	{
		E_RIGHT,
		E_LEFT
	};
private:
	vector<texture*>* m_pTextures;

	int m_iCurrentImage;
	int m_iNextImage;
	int m_iMaxImageCount;

	int  m_iChangeXPos;
	bool m_bChangeWay;

	int  m_iCurrentState;

	DWORD m_dwLastOperate;

public:
	// Kinect

	// Right Hand Info
	HandState m_handState[6];

	D3DXVECTOR2 m_vHandPosition[6];
	D3DXVECTOR2 m_vFirstLockPosition[6];
public:
	CPictures();
	virtual ~CPictures();

	void Init();
	void Update();
	void Render();
	void Release();

	void UpdateFrameTimer(DWORD dwDelay);

	void InputProcessing();

	void UpdateStateMachine();
	void RenderStateMachine();

	template <typename T>
	void lerp(T *dot, const T p1, const T p2, const float d = 0.05f)
	{
		*dot = p1 + (p2 - p1) * d;
	}
};

