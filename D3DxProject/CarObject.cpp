#include "stdafx.h"
#include "CarObject.h"
#include "Mesh.h"

CCarObject::CCarObject() {
	XMVECTOR minVect{ XMVectorSet(-2.0f, -2.0f, -2.0f, 1.0f) };
	XMVECTOR maxVect{ XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f) };

	BoundingBox::CreateFromPoints(boundBox, minVect, maxVect);
}

CCarObject::~CCarObject() {

}

void CCarObject::Animate(float elapsedTime) {
	CRotatingObject::Animate(elapsedTime);
	SetPosition(Vector3::Add(GetPosition(), Vector3::ScalarProduct(direction, elapsedTime)));
	
	if (this->GetPosition().x > 200.0f || this->GetPosition().x < -200.0f)
		direction.x = -direction.x;
	if (this->GetPosition().y > 200.0f || this->GetPosition().y < -200.0f)
		direction.y = -direction.y;
	if (this->GetPosition().z > 300.0f || this->GetPosition().z < -300.0f)
		direction.z = -direction.z;
	if (!isLive)
		deletedTime += elapsedTime;
}

bool CCarObject::CheckRayIntersection(const XMFLOAT3& rayPos, const XMFLOAT3& rayDir, float* distance) {
	XMVECTOR xmrayOrigin{ XMLoadFloat3(&rayPos) };
	XMVECTOR xmrayDir{ XMLoadFloat3(&rayDir) };
	float dist{};
	BoundingBox curColl{ boundBox };
	curColl.Transform(curColl, XMLoadFloat4x4(&m_xmf4x4World));
	return curColl.Intersects(xmrayOrigin, xmrayDir, dist);
}


CCarObjectShader::CCarObjectShader() {

}

CCarObjectShader::~CCarObjectShader() {
	for (auto& it : particleObject)
		it->Release();

	for (auto& it : particles)
		it->Release();

	for (auto& it : dieObject)
		it->Release();
}

void CCarObjectShader::CreateShaderVariables(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList) {
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
		(sizeof(VS_VB_INSTANCE) * maxEnemyBoxCount) + (sizeof(VS_VB_INSTANCE) * 1000), D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL);
	m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);

	for (int i = 0; i < maxEnemyBoxCount; ++i) {
		CCarObject* enemyBox{ new CCarObject{} };
		enemyBox->SetPosition(XMFLOAT3(rand() % 100, rand() % 100, rand() % 100));
		enemyBox->SetRotationSpeed(float(rand() % 100) / 1.1f);
		//enemyBox->SetRotatinAxis(XMFLOAT3(rand() % 90, rand() % 90, rand() % 90));
		enemyBox->SetMesh(enemyBoxMesh);
		enemyBox->SetDirection(XMFLOAT3{ RANDOM_VALUE(1), RANDOM_VALUE(1), RANDOM_VALUE(1) });
		m_ppObjects.push_back(enemyBox);
	}


	for (int i = 0; i < 1000; ++i) {
		CCarObjectParticle* particle{ new CCarObjectParticle{} };
		particle->SetMesh(particleBoxMesh);
		particle->SetDirection(XMFLOAT3{ -RANDOM_VALUE(1.0f) + RANDOM_VALUE(1.0f), -RANDOM_VALUE(1.0f) + RANDOM_VALUE(1.0f
		), -RANDOM_VALUE(1.0f) + RANDOM_VALUE(1.0f) });
		particles.push_back(particle);
	}
}

void CCarObjectShader::UpdateShaderVariables(ID3D12GraphicsCommandList
	* pd3dCommandList) {
	pd3dCommandList->SetGraphicsRootShaderResourceView(2,
		m_pd3dcbGameObjects->GetGPUVirtualAddress());

	int j{ 0 };
	for (auto& it : m_ppObjects) {
		XMFLOAT4X4 worldMat = it->Get_m_xmf4x4World();
		m_pcbMappedGameObjects[j].m_xmcColor = XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f };
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&worldMat)));
		++j;
	}
	for (auto& it : particleObject) {
		XMFLOAT4X4 worldMat = it->Get_m_xmf4x4World();
		m_pcbMappedGameObjects[j].m_xmcColor = XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f };
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
			XMMatrixTranspose(XMLoadFloat4x4(&worldMat)));
		++j;
	}
}

void CCarObjectShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList) {
	enemyBoxMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
		4.0f, 4.0f, 4.0f);
	particleBoxMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
		2.0f, 2.0f, 2.0f, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}


void CCarObjectShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera
	* pCamera) {
	CObjectsShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);
	if (m_ppObjects.size())
		m_ppObjects.front()->Render(pd3dCommandList, pCamera, m_ppObjects.size() + particleObject.size());
}

void CCarObjectShader::AnimateObjects(float elapsedTime) {
	auto end{ m_ppObjects.end() };
	for (auto it = m_ppObjects.begin(); it != end;) {
		CCarObject* box{ reinterpret_cast<CCarObject*>((*it)) };
		box->Animate(elapsedTime);
		if (!box->GetLive()) {
			dieObject.push_back(box);
			for (int i = 0; i < 100; ++i) {
				particles.back()->SetPosition(box->GetPosition());
				particles.back()->SetParent(box);
				particleObject.push_back(particles.back());
				particles.pop_back();
			}
			it = m_ppObjects.erase(it);
		}
		else
			++it;
	}
	for (auto& it : particleObject)
		it->Animate(elapsedTime);

	auto end2{ dieObject.end() };
	for (auto it = dieObject.begin(); it != end2; ) {
		(*it)->Animate(elapsedTime);
		if ((*it)->GetDeletedTime() > 3.0f) {
			particleObject.erase(std::remove_if(particleObject.begin(), particleObject.end(), [&](CCarObjectParticle* p) {
				return p->GetParent() == (*it);
				}), particleObject.end());
			m_ppObjects.push_back(*it);
			(*it)->SetLive(true);
			it = dieObject.erase(it);
		}
		else
			++it;
	}
}

void CCarObjectShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature) {
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

CGameObject* CCarObjectShader::IsPickingObject(const XMFLOAT3& rayDirOrigin, const XMFLOAT3& rayDir) {
	for (auto& it : m_ppObjects) {
		CCarObject* box{ reinterpret_cast<CCarObject*>(it) };
		if (box->CheckRayIntersection(rayDirOrigin, rayDir, nullptr))
			return box;
	}
	return nullptr;
}

CCarObjectParticle::CCarObjectParticle() {

}

CCarObjectParticle::~CCarObjectParticle() {

}

void CCarObjectParticle::Animate(float elapsedTime) {
	CGameObject::Animate(elapsedTime);
	SetPosition(Vector3::Add(GetPosition(), Vector3::ScalarProduct(direction, elapsedTime)));
}
