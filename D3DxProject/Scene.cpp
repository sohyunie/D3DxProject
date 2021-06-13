#include "stdafx.h"
#include "Scene.h"
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
#include "CarObject.h"
#include "Player.h"

CScene::CScene() {

}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList)
{
	//m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	//m_nShaders = 1;
	//m_pShaders = new CInstancingShader[m_nShaders];
	//m_pShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//m_pShaders[0].BuildObjects(pd3dDevice, pd3dCommandList);



	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CInstancingShader* shader{ new CInstancingShader{} };
	shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	shader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pShaders.push_back(shader);
	shader = new CCarObjectShader{};
	shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	shader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pShaders.push_back(shader);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	for (auto& it : m_pShaders) {
		it->ReleaseShaderVariables();
		it->ReleaseObjects();
		delete it;
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (auto& it : m_pShaders)
		it->ReleaseUploadBuffers();
}

void CScene::SetPlayer(CPlayer* player)
{
	this->m_pPlayer = player;
}

// OBJECT를 앞으로 보낼 곳
void CScene::AnimateObjects(float fTimeElapsed)
{
	for (auto& it : m_pShaders)
		it->AnimateObjects(fTimeElapsed, this->m_pPlayer->GetPosition());

	// Collision
	for (auto& it : m_pShaders) {
		for (auto& objs : it->GetGameObject()) {
			if (objs->type == ObjectType::CAR) {
				ContainmentType containType = m_pPlayer->m_xmOOBB.Contains(objs->m_xmOOBB);
				if (objs->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
				{
					if (!m_pPlayer->m_bBooster) {
						m_pPlayer->m_speed = SPEED_DEFAULT;
						if ((dynamic_cast<CCarObject*>(objs)->GetLive()))
						{
							dynamic_cast<CCarObject*>(objs)->SetLive(false);
							m_pPlayer->m_hp -= 5;
						}
					}
				}
			}
		}
	}
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->UpdateShaderVariables(pd3dCommandList);
	for (auto& it : m_pShaders)
		it->Render(pd3dCommandList, pCamera);
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