#pragma once

// 카메라의 종류(모드: Mode)를 나타내는 상수를 다음과 같이 선언한다.
#define FIRST_PERSON_CAMERA 0x01
#define SPACESHIP_CAMERA 0x02
#define THIRD_PERSON_CAMERA 0x03

//프레임 버퍼의 크기와 종횡비(Aspect Ratio)를 나타내는 상수를 다음과 같이 선언한다.
#define ASPECT_RATIO (float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

class CPlayer;

struct VS_CB_CAMERA_INFO
{
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;
};

class CCamera
{
public:
	CCamera();
	CCamera(CCamera* pCamera);
	virtual ~CCamera();

protected:
	// 카메라의 위치(월드좌표계) 벡터이다.
	XMFLOAT3 m_xmf3Position{};

	// 카메라의 로컬 x-축(Right), y-축(Up), z-축(Look)을 나타내는 벡터이다.
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.0f, 0.0f, 0.0f) };
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.0f, 1.0f, 0.0f) };
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.0f, 0.0f, 1.0f) };

	// 카메라가 x-축, z-축, y-축으로 얼마만큼 회전했는 가를 나타내는 각도이다.
	float m_fPitch{};
	float m_fRoll{};
	float m_fYaw{};

	// 카메라의 종류(1인칭 카메라, 스페이스-쉽 카메라, 3인칭 카메라)를 나타낸다.
	DWORD m_nMode{};

	// 플레이어가 바라볼 위치 벡터이다. 주로 3인칭 카메라에서 사용된다.
	XMFLOAT3 m_xmf3LookAtWorld{};

	// 플레이어와 카메라의 오프셋을 나타내는 벡터이다. 주로 3인칭 카메라에서 사용된다.
	XMFLOAT3 m_xmf3Offset{};

	// 플레이어가 회전할 때 얼마만큼의 시간을 지연시킨 후 카메라를 회전시킬 것인가를 나타낸다.
	float m_fTimeLag{};

	// 카메라 변환 행렬
	XMFLOAT4X4 m_xmf4x4View{ Matrix4x4::Identity() };

	// 투영 변환 행렬
	XMFLOAT4X4 m_xmf4x4Projection{ Matrix4x4::Identity() };

	// 뷰포트와 씨저 사각형
	D3D12_VIEWPORT m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	// 카메라를 가지고 있는 플레이어에 대한 포인터이다.
	CPlayer* m_pPlayer{};

public:
	// 카메라의 정보를 셰이더 프로그램에게 전달하기 위한 상수 버퍼를 생성하고 갱신한다.
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	// 카메라 변환 행렬을 생성한다.
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);

	// 카메라가 여러번 회전을 하게 되면 누적된 실수 연산의 부정확성 때문에 카메라의 로컬 x-축(Right), y-축(Up), z-축(Look)이
	// 서로 직교하지 않을 수 있다. 카메라의 로컬 x-축(Right), y-축(Up), z-축(Look)이 서로 직교하도록 만들어준다.
	void RegenerateViewMatrix();

	// 투영 변환 행렬을 생성한다.
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);
	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetPlayer(CPlayer* pPlayer);
	CPlayer* GetPlayer();

	void SetMode(DWORD nMode);
	DWORD GetMode();

	void SetPosition(XMFLOAT3 xmf3Position);
	XMFLOAT3& GetPosition();

	void SetLookAtPosition(XMFLOAT3 xmf3LookAtWorld);
	XMFLOAT3& GetLookAtPosition();

	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();
	XMFLOAT3& GetLookVector();

	float& GetPitch();
	float& GetRoll();
	float& GetYaw();

	void SetOffset(XMFLOAT3 xmf3Offset);
	XMFLOAT3& GetOffset();

	void SetTimeLag(float fTimeLag);
	float GetTimeLag();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();

	D3D12_VIEWPORT GetViewport();
	D3D12_RECT GetScissorRect();

	// 카메라를 xmf3Shift 만큼 이동한다.
	virtual void Move(const XMFLOAT3& xmf3Shift);

	// 카메라를 x-축, y-축, z-축으로 회전하는 가상함수이다.
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	// 카메라의 이동, 회전에 따라 카메라의 정보를 갱신하는 가상함수이다.
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);

	// 3인칭 카메라에서 카메라가 바라보는 지점을 설정한다. 일반적으로 플레이어를 바라보도록 설정한다.
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt);
};

class CSpaceShipCamera : public CCamera
{
public:
	CSpaceShipCamera(CCamera* pCamera);
	virtual ~CSpaceShipCamera();

public:
	// 스페이스-쉽 카메라를 플레이어의 로컬 x-축(Right), y-축(Up), z-축(Look)을 기준으로 회전하는 함수이다.
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CFirstPersonCamera : public CCamera
{
public:
	CFirstPersonCamera(CCamera* pCamera);
	virtual ~CFirstPersonCamera();

public:
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CCamera* pCamera);
	virtual ~CThirdPersonCamera();

public:
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(XMFLOAT3& vLookAt);
};
