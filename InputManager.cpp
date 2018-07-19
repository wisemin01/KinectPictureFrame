#include "DXUT.h"

CInputManager::CInputManager()
	:m_pKinectSensor(nullptr),
	m_pCoordinateMapper(nullptr),
	m_pBodyFrameReader(nullptr)
{
	m_vLHandPosition[0] = D3DXVECTOR2(0, 0);
	m_vRHandPosition[0] = D3DXVECTOR2(0, 0);

	KinectInit();
}


CInputManager::~CInputManager()
{
	KinectRelease();
}

void CInputManager::KinectInit()
{
	InitSensor();
}

void CInputManager::KinectRelease()
{
}

void CInputManager::InputUpdate()
{
	// BodyFrame을 읽어 올 수 있는지 확인합니다.
	if (!m_pBodyFrameReader)
		return;

	IBodyFrame * pBodyFrame = nullptr;

	// 현재 프레임의 인식된 몸들을 가져옵니다.
	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		// 현재 프레임이 갱신된 시간을 가져옵니다.
		INT64 nTime = 0;
		hr = pBodyFrame->get_RelativeTime(&nTime);

		// 현재 인식한 몸의 정보를 저장하는 배열입니다. (최대 6개)
		IBody * ppBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
			// 몸의 정보를 저장하는 배열에 현재 인식한 몸들을 저장합니다.
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}
		if (SUCCEEDED(hr))
		{
			ProcessBody(nTime, BODY_COUNT, ppBodies);
		}

		//다 사용했다면 요번프레임의 몸들은 해제해줍니다
		for (int i = 0; i < _countof(ppBodies); ++i)
			SAFE_RELEASE(ppBodies[i]);
	}

	SAFE_RELEASE(pBodyFrame);
}

HRESULT CInputManager::ProcessBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	HRESULT hr;

	if (m_pCoordinateMapper)
	{
		//각각 몸의 정보를 Joint정보를 설정합니다.
		for (int i = 0; i < 6; ++i)
		{
			hr = SetJointState(ppBodies[i], i);
		}
	}

	return hr;
}


const D3DXVECTOR2 & CInputManager::GetHandPosition(int iBodyCount, HAND_SET iHandSet)
{
	return iHandSet == HAND_SET::E_HAND_LEFT ? m_vLHandPosition[iBodyCount] : m_vRHandPosition[iBodyCount];
}

const HandState & CInputManager::GetHandState(int iBodyCount, HAND_SET iHandSet)
{
	return iHandSet == HAND_SET::E_HAND_LEFT ? m_sLHandState[iBodyCount] : m_sRHandState[iBodyCount];
}

HRESULT CInputManager::InitSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		IBodyFrameSource* pBodyFrameSource = nullptr;

		// 키넥트 센서 오픈
		hr = m_pKinectSensor->Open();
		if (SUCCEEDED(hr))
		{
			// 오픈된 키넥트 센서에서 CoordinateMapper를 가져옵니다.
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			// 오픈된 키넥트 센서에서 BodyFrameSource를 가져옵니다.
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			// 오픈된 키넥트 센서에서 Reader를 가져옵니다.
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		SAFE_RELEASE(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		MessageBox(nullptr, L"No Ready Kinect found!", L"[!] Error", MB_OK);
		return E_FAIL;
	}

	return hr;
}

const D3DXVECTOR2 & CInputManager::BodyToScreen(const CameraSpacePoint & bodyPoint, int width, int height)
{
	DepthSpacePoint depthPoint = { 0 };

	m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

	// 변환된 값 저장용 변수
	float screeonPointX = static_cast<float>(depthPoint.X * width) / DEPTH_WIDTH;
	float screeonPointY = static_cast<float>(depthPoint.Y * height) / DEPTH_HEIGHT;

	return D3DXVECTOR2(screeonPointX, screeonPointY);
}

HRESULT CInputManager::SetJointState(IBody * pBody, int iBodyCount)
{
	if (!pBody)
		return E_FAIL;

	HRESULT hr;

	BOOLEAN bTracked = false;
	hr	= pBody->get_IsTracked(&bTracked);

	if (SUCCEEDED(hr))
	{
		// 손의 상태를 초기화 해줍니다.
		m_sLHandState[iBodyCount] = HandState_Unknown;
		m_sRHandState[iBodyCount] = HandState_Unknown;

		// 손의 상태를 얻어옵니다 
		pBody->get_HandLeftState(&m_sLHandState[iBodyCount]);
		pBody->get_HandRightState(&m_sRHandState[iBodyCount]);

		// 현재 Set하는 몸의 관절 데이터를 받아옵니다.
		hr = pBody->GetJoints(_countof(m_JointData[iBodyCount]), m_JointData[iBodyCount]);
		if (SUCCEEDED(hr))
		{

			// 관절들의 위치를 스크린 좌표로 변환합니다.
			for (int i = 0; i < _countof(m_JointData[iBodyCount]); ++i)
			{
				m_vJointsPosition[iBodyCount][i] = BodyToScreen(m_JointData[iBodyCount][i].Position);
			}

			// 손들의 위치를 저장합니다.
			m_vLHandPosition[iBodyCount] = m_vJointsPosition[iBodyCount][JointType_HandLeft];
			m_vRHandPosition[iBodyCount] = m_vJointsPosition[iBodyCount][JointType_HandRight];
		}
	}


	return hr;
}
