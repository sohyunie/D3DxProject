#include "stdafx.h"
#include "Scene.h"
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
#include "CarObject.h"
#include "CoinObject.h"
#include "ItemObject.h"
#include "Player.h"

CScene::CScene() {

}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	//���� Ȯ���� ������ �����̴�. x-��� z-���� 8��, y-���� 2�� Ȯ���Ѵ�. 
	XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.2f, 0.0f, 0.0f);
	//������ ���� �� �̹��� ����(HeightMap.raw)�� ����Ͽ� �����Ѵ�. ���� ���� ũ��� ����x����(257x257)�̴�. 
#ifdef _WITH_TERRAIN_PARTITION
	/*�ϳ��� ���� �޽��� ũ��� ����x����(17x17)�̴�. ���� ��ü�� ���� �������� 16��, ���� �������� 16�� ���� ��
	���� ������. ������ �����ϴ� ���� �޽��� ������ �� 256(16x16)���� �ȴ�.*/
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList,
		m_pd3dGraphicsRootSignature, _T("../Assets/Image/Terrain/HeightMap.raw"), 257, 257, 17,
		17, xmf3Scale, xmf4Color);
#else
//������ �ϳ��� ���� �޽�(257x257)�� �����Ѵ�. 
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList,
		m_pd3dGraphicsRootSignature, _T("../Assets/Image/Terrain/HeightMap.raw"), 257, 257, 257,
		257, xmf3Scale, xmf4Color);
#endif
	m_nShaders = 1;
	m_pTerrainShader = new CObjectsShader[m_nShaders];
	m_pTerrainShader[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pTerrainShader[0].BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain);

	//// Wall
	//CInstancingShader* shader{ new CInstancingShader{} };
	//shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//shader->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
	//m_pShaders.push_back(shader);
	//// Car Object
	//shader = new CCarObjectShader{};
	//shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//shader->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
	//m_pShaders.push_back(shader);
	//// Coin Object
	//shader = new CoinObjectShader{};
	//shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//shader->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
	//m_pShaders.push_back(shader);
	//// Item Object
	//shader = new ItemObjectShader{};
	//shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//shader->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
	//m_pShaders.push_back(shader);

	//// Terrain
	//shader = new CInstancingShader{};
	//shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//shader->BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain);
	//m_pShaders.push_back(shader);
}

void CScene::ReleaseObjects()
{
	/*if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	for (auto& it : m_pShaders) {
		it->ReleaseShaderVariables();
		it->ReleaseObjects();
		delete it;
	}*/
	if (m_pTerrain) delete m_pTerrain;
}

void CScene::ReleaseUploadBuffers()
{
	/*for (auto& it : m_pShaders)
		it->ReleaseUploadBuffers();*/

	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
}

void CScene::SetPlayer(CPlayer* player)
{
	this->m_pPlayer = player;
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	//for (auto& it : m_pShaders)
	//	it->AnimateObjects(fTimeElapsed, this->m_pPlayer->GetPosition());

	//// Collision
	//for (auto& it : m_pShaders) {
	//	for (auto& objs : it->GetGameObject()) {
	//		switch (objs->type) {
	//		case ObjectType::CAR:
	//		{
	//			ContainmentType containType = m_pPlayer->m_xmOOBB.Contains(objs->m_xmOOBB);
	//			if (objs->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
	//			{
	//				if (!m_pPlayer->m_bBooster) {
	//					m_pPlayer->m_speed = SPEED_DEFAULT;
	//					if ((dynamic_cast<CCarObject*>(objs)->GetLive()))
	//					{
	//						dynamic_cast<CCarObject*>(objs)->SetLive(false);
	//						m_pPlayer->m_hp -= 5;
	//					}
	//				}
	//			}
	//		}
	//		break;
	//		case ObjectType::COIN:
	//		{
	//			ContainmentType containType = m_pPlayer->m_xmOOBB.Contains(objs->m_xmOOBB);
	//			if (objs->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
	//			{
	//				if ((dynamic_cast<CoinObject*>(objs)->GetLive()))
	//				{
	//					dynamic_cast<CoinObject*>(objs)->SetLive(false);
	//					m_pPlayer->coin++;
	//				}
	//			}
	//		}
	//		break;
	//		case ObjectType::ITEM:
	//		{
	//			ContainmentType containType = m_pPlayer->m_xmOOBB.Contains(objs->m_xmOOBB);
	//			if (objs->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
	//			{
	//				if ((dynamic_cast<ItemObject*>(objs)->GetLive()))
	//				{
	//					dynamic_cast<ItemObject*>(objs)->SetLive(false);
	//					
	//					m_pPlayer->m_bBooster = true;
	//					m_pPlayer->m_boostTime = 3.0f;
	//					m_pPlayer->m_speed = 400;
	//				}
	//			}
	//		}
	//		break;
	//		}
	//	}
	//}
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	/*for (auto& it : m_pShaders)
		it->Render(pd3dCommandList, pCamera);*/

	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);
	if (m_pTerrainShader) m_pTerrainShader->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_pShaders.size(); i++)
	{
		m_pShaders.at(i)->Render(pd3dCommandList, pCamera);
		//m_pShaders[i].Render();
	}
}

ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;
	D3D12_ROOT_PARAMETER pd3dRootParameters[3];
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Constants.Num32BitValues = 16;
	pd3dRootParameters[0].Constants.ShaderRegister = 0; //b0: Player
	pd3dRootParameters[0].Constants.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 1; //b1: Camera
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 0; //t0
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;
	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void
			**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
	return(pd3dGraphicsRootSignature);
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
	lParam)
{
	return(false);
}
bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam,
	LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return(false);
}


ID3D12RootSignature* CScene::GetGraphicsRootSignature()
{
	return(m_pd3dGraphicsRootSignature);
}