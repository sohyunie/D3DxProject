#pragma once
#include "Timer.h"
#include "Shader.h"

class CScene
{
public:
	CScene();
	~CScene();

	CPlayer* m_pPlayer = NULL;
	CGameObject** m_ppObjects = NULL;
protected:
	bool isOver = false;
	CHeightMapTerrain* m_pTerrain = NULL;
public:
	CHeightMapTerrain* GetTerrain() { return(m_pTerrain); }

protected:
	// ��ġ(Batch) ó���� �ϱ� ���Ͽ� ���� ���̴����� ����Ʈ�� ǥ���Ѵ�.
	CObjectsShader* m_pShaders{};
	int m_nShaders{};
	int m_nObjects{};
	// ��Ʈ �ñ׳��ĸ� ��Ÿ���� �������̽� �������̴�.
	ID3D12RootSignature* m_pd3dGraphicsRootSignature{};

public:
	// ������ ���콺�� Ű���� �޽����� ó���Ѵ�.
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
	void ReleaseObjects();

	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	void CheckObjectByPlayer();
	void CheckObjectByBullet();
	void ReleaseUploadBuffers();

	// �׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();
};
