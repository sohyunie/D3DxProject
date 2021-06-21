#include "stdafx.h"
#include "ItemObject.h"
#include "GameObject.h"
#include "Mesh.h"

ItemObject::ItemObject() {
	this->type = ObjectType::ITEM;
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 200.0f;
}

ItemObject::~ItemObject() {

}

void ItemObject::Animate(float elapsedTime, XMFLOAT3 playerPos) {
	//CoinObject::Animate(elapsedTime, playerPos);
	CRotatingObject::Animate(elapsedTime, playerPos);


	int bufferDistance = 50;
	if (this->GetPosition().z < playerPos.z - bufferDistance && this->GetLive()) {
		this->SetPosition(this->GetPosition().x, 0, playerPos.z + 1000);
	}

	SetPosition(Vector3::Add(GetPosition(), Vector3::ScalarProduct(direction, elapsedTime)));

	if (!isLive) {
		deletedTime += elapsedTime;
		if (this->GetDeletedTime() > 3.0f) {
			this->SetLive(true);
		}
	}

	this->m_xmf4x4World._42 = (!this->isLive) ? -1000 : 0;
}

ItemObjectShader::ItemObjectShader() {

}

ItemObjectShader::~ItemObjectShader() {
	
}

void ItemObjectShader::CreateShaderVariables(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList) {
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		(sizeof(VS_VB_INSTANCE) * itemCount), D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);

	for (int i = 0; i < itemCount; ++i) {
		ItemObject* itemObj{ new ItemObject{} };
		itemObj->SetPosition(XMFLOAT3(-100 + rand() % 200, 0, 400 + rand() % 1000));
		itemObj->SetMesh(0, itemBoxMesh);
		itemObj->SetDirection(XMFLOAT3{ 0,0, RANDOM_VALUE(1) });
		m_ppObjects.push_back(itemObj);
	}
}

void ItemObjectShader::UpdateShaderVariables(ID3D12GraphicsCommandList
	* pd3dCommandList) {
	pd3dCommandList->SetGraphicsRootShaderResourceView(2,
		m_pd3dcbGameObjects->GetGPUVirtualAddress());

	int j{ 0 };
	for (auto& it : m_ppObjects) {
		XMFLOAT4X4 worldMat = it->Get_m_xmf4x4World();
		m_pcbMappedGameObjects[j].m_xmcColor = XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&worldMat)));
		++j;
	}
}

void ItemObjectShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList) {
	itemBoxMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
		4.0f, 4.0f, 4.0f);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}


void ItemObjectShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera
	* pCamera) {
	CObjectsShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);
	if (m_ppObjects.size())
		m_ppObjects.front()->Render(pd3dCommandList, pCamera, m_ppObjects.size());
}

void ItemObjectShader::AnimateObjects(float elapsedTime, XMFLOAT3 playerPos) {
	for (auto it = m_ppObjects.begin(); it != m_ppObjects.end();) {
		ItemObject* item{ reinterpret_cast<ItemObject*>((*it)) };
		item->Animate(elapsedTime, playerPos);
		++it;
	}
}

void ItemObjectShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature) {
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}