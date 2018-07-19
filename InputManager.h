#pragma once
#include "singleton.h"

// ==============================================================
// InputManager ���� ��ũ�� ����
// ==============================================================

// �ѹ��� �ν� �� �� �ִ� ���� ����
#define BODY_COUNT 6

class CInputManager :
	public singleton<CInputManager>
{
public:

	// ==============================================================
	// Enum ���� ����
	// ==============================================================

	// ���� ��ġ ���� ( ������, ���� )
	enum HAND_SET
	{
		E_HAND_LEFT,
		E_HAND_RIGHT
	};
private:

	// ==============================================================
	// �ɹ� ��ü
	// ==============================================================

	// KinectScenor : ������ �̿��ϱ� ���� ��ü. 
	IKinectSensor		*	m_pKinectSensor;

	// MapPer		: ������ �����ִ� ��ü.
	ICoordinateMapper   *	m_pCoordinateMapper;
	
	// FrameReader	: �ش� �������� ���� ������ ���� �� �ִ� ����.
	IBodyFrameReader	*   m_pBodyFrameReader;

	// ������ ������ ��� ��ü�� �迭�Դϴ�.
	Joint					m_JointData[BODY_COUNT][JointType_Count];

	// ������ ��ġ�� ��ũ�� ��ġ�� ��ȯ�� ��ġ�� �迭�Դϴ�.
	D3DXVECTOR2				m_vJointsPosition[BODY_COUNT][JointType_Count];

	// ���� ���¸� �����ϴ� ���� �Դϴ�.
	HandState				m_sLHandState[BODY_COUNT];
	HandState				m_sRHandState[BODY_COUNT];

	// �޼� ��ġ ���� �� ��ġ.
	D3DXVECTOR2				m_vLHandPosition[BODY_COUNT];
	D3DXVECTOR2				m_vRHandPosition[BODY_COUNT];
public:
	CInputManager();
	~CInputManager();

	// ==============================================================
	// InputManager ���� �Լ� ����
	// ==============================================================

	// Kinect ��ü ����
	void KinectInit();
	void KinectRelease();

	// �� ������ ����
	void	InputUpdate();
	
	// ������ ���� SetJointState�� �����ϴ� �Լ��Դϴ�.
	HRESULT ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

	// ==============================================================
	// ������ ������ �Լ� ����
	// ==============================================================

	// ���� ��ġ ������ ���� ( ���� : ���� ��ġ )
	const D3DXVECTOR2&	GetHandPosition(int iBodyCount, HAND_SET iHandSet);
	// ���� ���� ������ ���� ( ���� : ���� ��ġ )
	const HandState&	GetHandState(int iBodyCount, HAND_SET iHandSet);

private:

	// ==============================================================
	// �ɹ� ��ü ���� �Լ� ����
	// ==============================================================

	HRESULT InitSensor();

private:
	// ==============================================================
	// ��ƿ��Ƽ �Լ� ����
	// ==============================================================

	// ���� �� ��ġ�� ȭ����� ��ǥ�� ��ȯ�մϴ�.
	const D3DXVECTOR2 & BodyToScreen(const CameraSpacePoint& bodyPoint, int width = WINSIZEX, int height = WINSIZEY);
	// ������ ���¿� ��ġ�� �����ϴ� �Լ��Դϴ�.
	HRESULT				SetJointState(IBody * pBody, int iBodyCount);
	
};

#define INPUTMANAGER CInputManager::Instance()