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
	// BodyFrame�� �о� �� �� �ִ��� Ȯ���մϴ�.
	if (!m_pBodyFrameReader)
		return;

	IBodyFrame * pBodyFrame = nullptr;

	// ���� �������� �νĵ� ������ �����ɴϴ�.
	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		// ���� �������� ���ŵ� �ð��� �����ɴϴ�.
		INT64 nTime = 0;
		hr = pBodyFrame->get_RelativeTime(&nTime);

		// ���� �ν��� ���� ������ �����ϴ� �迭�Դϴ�. (�ִ� 6��)
		IBody * ppBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
			// ���� ������ �����ϴ� �迭�� ���� �ν��� ������ �����մϴ�.
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}
		if (SUCCEEDED(hr))
		{
			ProcessBody(nTime, BODY_COUNT, ppBodies);
		}

		//�� ����ߴٸ� ����������� ������ �������ݴϴ�
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
		//���� ���� ������ Joint������ �����մϴ�.
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

		// Ű��Ʈ ���� ����
		hr = m_pKinectSensor->Open();
		if (SUCCEEDED(hr))
		{
			// ���µ� Ű��Ʈ �������� CoordinateMapper�� �����ɴϴ�.
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			// ���µ� Ű��Ʈ �������� BodyFrameSource�� �����ɴϴ�.
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			// ���µ� Ű��Ʈ �������� Reader�� �����ɴϴ�.
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

	// ��ȯ�� �� ����� ����
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
		// ���� ���¸� �ʱ�ȭ ���ݴϴ�.
		m_sLHandState[iBodyCount] = HandState_Unknown;
		m_sRHandState[iBodyCount] = HandState_Unknown;

		// ���� ���¸� ���ɴϴ� 
		pBody->get_HandLeftState(&m_sLHandState[iBodyCount]);
		pBody->get_HandRightState(&m_sRHandState[iBodyCount]);

		// ���� Set�ϴ� ���� ���� �����͸� �޾ƿɴϴ�.
		hr = pBody->GetJoints(_countof(m_JointData[iBodyCount]), m_JointData[iBodyCount]);
		if (SUCCEEDED(hr))
		{

			// �������� ��ġ�� ��ũ�� ��ǥ�� ��ȯ�մϴ�.
			for (int i = 0; i < _countof(m_JointData[iBodyCount]); ++i)
			{
				m_vJointsPosition[iBodyCount][i] = BodyToScreen(m_JointData[iBodyCount][i].Position);
			}

			// �յ��� ��ġ�� �����մϴ�.
			m_vLHandPosition[iBodyCount] = m_vJointsPosition[iBodyCount][JointType_HandLeft];
			m_vRHandPosition[iBodyCount] = m_vJointsPosition[iBodyCount][JointType_HandRight];
		}
	}


	return hr;
}
