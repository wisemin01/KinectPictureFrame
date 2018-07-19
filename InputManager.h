#pragma once
#include "singleton.h"

// ==============================================================
// InputManager 관련 매크로 모음
// ==============================================================

// 한번에 인식 할 수 있는 몸의 갯수
#define BODY_COUNT 6

class CInputManager :
	public singleton<CInputManager>
{
public:

	// ==============================================================
	// Enum 선언 모음
	// ==============================================================

	// 손의 위치 정보 ( 오른쪽, 왼쪽 )
	enum HAND_SET
	{
		E_HAND_LEFT,
		E_HAND_RIGHT
	};
private:

	// ==============================================================
	// 맴버 객체
	// ==============================================================

	// KinectScenor : 센서를 이용하기 위한 객체. 
	IKinectSensor		*	m_pKinectSensor;

	// MapPer		: 맵핑을 도와주는 객체.
	ICoordinateMapper   *	m_pCoordinateMapper;
	
	// FrameReader	: 해당 프레임의 관절 정보를 읽을 수 있는 도구.
	IBodyFrameReader	*   m_pBodyFrameReader;

	// 관절의 정보를 담는 객체의 배열입니다.
	Joint					m_JointData[BODY_COUNT][JointType_Count];

	// 관절의 위치를 스크린 위치로 변환한 위치의 배열입니다.
	D3DXVECTOR2				m_vJointsPosition[BODY_COUNT][JointType_Count];

	// 손의 상태를 저장하는 변수 입니다.
	HandState				m_sLHandState[BODY_COUNT];
	HandState				m_sRHandState[BODY_COUNT];

	// 왼손 위치 오른 손 위치.
	D3DXVECTOR2				m_vLHandPosition[BODY_COUNT];
	D3DXVECTOR2				m_vRHandPosition[BODY_COUNT];
public:
	CInputManager();
	~CInputManager();

	// ==============================================================
	// InputManager 관리 함수 모음
	// ==============================================================

	// Kinect 객체 관리
	void KinectInit();
	void KinectRelease();

	// 매 프레임 갱신
	void	InputUpdate();
	
	// 각각의 몸에 SetJointState를 실행하는 함수입니다.
	HRESULT ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

	// ==============================================================
	// 정보를 얻어오는 함수 모음
	// ==============================================================

	// 손의 위치 정보를 얻어옴 ( 인자 : 손의 위치 )
	const D3DXVECTOR2&	GetHandPosition(int iBodyCount, HAND_SET iHandSet);
	// 손의 상태 정보를 얻어옴 ( 인자 : 손의 위치 )
	const HandState&	GetHandState(int iBodyCount, HAND_SET iHandSet);

private:

	// ==============================================================
	// 맴버 객체 관리 함수 모음
	// ==============================================================

	HRESULT InitSensor();

private:
	// ==============================================================
	// 유틸리티 함수 모음
	// ==============================================================

	// 몸의 점 위치를 화면상의 좌표로 변환합니다.
	const D3DXVECTOR2 & BodyToScreen(const CameraSpacePoint& bodyPoint, int width = WINSIZEX, int height = WINSIZEY);
	// 관절의 상태와 위치를 저장하는 함수입니다.
	HRESULT				SetJointState(IBody * pBody, int iBodyCount);
	
};

#define INPUTMANAGER CInputManager::Instance()