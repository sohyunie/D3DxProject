#pragma once

// ī�޶��� ����(���: Mode)�� ��Ÿ���� ����� ������ ���� �����Ѵ�.
#define FIRST_PERSON_CAMERA 0x01
#define SPACESHIP_CAMERA 0x02
#define THIRD_PERSON_CAMERA 0x03

//������ ������ ũ��� ��Ⱦ��(Aspect Ratio)�� ��Ÿ���� ����� ������ ���� �����Ѵ�.
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
	// ī�޶��� ��ġ(������ǥ��) �����̴�.
	XMFLOAT3 m_xmf3Position{};

	// ī�޶��� ���� x-��(Right), y-��(Up), z-��(Look)�� ��Ÿ���� �����̴�.
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.0f, 0.0f, 0.0f) };
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.0f, 1.0f, 0.0f) };
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.0f, 0.0f, 1.0f) };

	// ī�޶� x-��, z-��, y-������ �󸶸�ŭ ȸ���ߴ� ���� ��Ÿ���� �����̴�.
	float m_fPitch{};
	float m_fRoll{};
	float m_fYaw{};

	// ī�޶��� ����(1��Ī ī�޶�, �����̽�-�� ī�޶�, 3��Ī ī�޶�)�� ��Ÿ����.
	DWORD m_nMode{};

	// �÷��̾ �ٶ� ��ġ �����̴�. �ַ� 3��Ī ī�޶󿡼� ���ȴ�.
	XMFLOAT3 m_xmf3LookAtWorld{};

	// �÷��̾�� ī�޶��� �������� ��Ÿ���� �����̴�. �ַ� 3��Ī ī�޶󿡼� ���ȴ�.
	XMFLOAT3 m_xmf3Offset{};

	// �÷��̾ ȸ���� �� �󸶸�ŭ�� �ð��� ������Ų �� ī�޶� ȸ����ų ���ΰ��� ��Ÿ����.
	float m_fTimeLag{};

	// ī�޶� ��ȯ ���
	XMFLOAT4X4 m_xmf4x4View{ Matrix4x4::Identity() };

	// ���� ��ȯ ���
	XMFLOAT4X4 m_xmf4x4Projection{ Matrix4x4::Identity() };

	// ����Ʈ�� ���� �簢��
	D3D12_VIEWPORT m_d3dViewport{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	D3D12_RECT m_d3dScissorRect{ 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	// ī�޶� ������ �ִ� �÷��̾ ���� �������̴�.
	CPlayer* m_pPlayer{};

public:
	// ī�޶��� ������ ���̴� ���α׷����� �����ϱ� ���� ��� ���۸� �����ϰ� �����Ѵ�.
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	// ī�޶� ��ȯ ����� �����Ѵ�.
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);

	// ī�޶� ������ ȸ���� �ϰ� �Ǹ� ������ �Ǽ� ������ ����Ȯ�� ������ ī�޶��� ���� x-��(Right), y-��(Up), z-��(Look)��
	// ���� �������� ���� �� �ִ�. ī�޶��� ���� x-��(Right), y-��(Up), z-��(Look)�� ���� �����ϵ��� ������ش�.
	void RegenerateViewMatrix();

	// ���� ��ȯ ����� �����Ѵ�.
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

	// ī�޶� xmf3Shift ��ŭ �̵��Ѵ�.
	virtual void Move(const XMFLOAT3& xmf3Shift);

	// ī�޶� x-��, y-��, z-������ ȸ���ϴ� �����Լ��̴�.
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	// ī�޶��� �̵�, ȸ���� ���� ī�޶��� ������ �����ϴ� �����Լ��̴�.
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);

	// 3��Ī ī�޶󿡼� ī�޶� �ٶ󺸴� ������ �����Ѵ�. �Ϲ������� �÷��̾ �ٶ󺸵��� �����Ѵ�.
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt);
};

class CSpaceShipCamera : public CCamera
{
public:
	CSpaceShipCamera(CCamera* pCamera);
	virtual ~CSpaceShipCamera();

public:
	// �����̽�-�� ī�޶� �÷��̾��� ���� x-��(Right), y-��(Up), z-��(Look)�� �������� ȸ���ϴ� �Լ��̴�.
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
