#pragma once
#include "Shader.h"
#include "GameObject.h"

class CCarObject : public  CRotatingObject {
public:
	CCarObject();
	~CCarObject();
	void SetDirection(const XMFLOAT3& dir) {
		XMFLOAT3 tempDir = dir;
		direction = Vector3::Normalize(tempDir);
	}
	virtual void Animate(float elapsedTime) override;

	bool CheckRayIntersection(const XMFLOAT3& rayPos, const XMFLOAT3& rayDir, float* distance);
	bool isCollision(const BoundingBox& target) {
		BoundingBox curColl{ boundBox };
		curColl.Transform(curColl, XMLoadFloat4x4(&m_xmf4x4World));
		return curColl.Contains(target) != ContainmentType::DISJOINT;
	}
	void SetLive(bool bFlag) {
		isLive = bFlag;
		deletedTime = 0.0f;
	}
	bool GetLive() {
		return isLive;
	}
	float GetDeletedTime() {
		return deletedTime;
	}
private:
	XMFLOAT3 direction{};
	BoundingBox boundBox{};
	bool isLive{ true };
	float deletedTime{ 0.0f };
};

class CCarObjectParticle : public CGameObject {
public:
	CCarObjectParticle();
	~CCarObjectParticle();

	virtual void Animate(float elapsedTime) override;

	void SetDirection(const XMFLOAT3& dir) {
		XMFLOAT3 tempDir = dir;
		direction = Vector3::Normalize(tempDir);
	}

	void SetParent(CCarObject* target) {
		parent = target;
	}
	CCarObject* GetParent() {
		return parent;
	}
private:
	XMFLOAT3 direction{};
	CCarObject* parent{ nullptr };
};

class CCarObjectShader : public CInstancingShader {
public:
	CCarObjectShader();
	virtual ~CCarObjectShader();
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	virtual void AnimateObjects(float elapsedTime) override;
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature) override;
	CGameObject* IsPickingObject(const XMFLOAT3& rayDirOrigin, const XMFLOAT3& rayDir);

	std::vector<CGameObject*>& GetGameObject() {
		return m_ppObjects;
	}
protected:

	ID3D12Resource* m_pd3dcbGameObjects = NULL;
	VS_VB_INSTANCE* m_pcbMappedGameObjects = NULL;

	std::vector<CCarObjectParticle*> particles{};
	std::vector<CCarObjectParticle*> particleObject{};
	std::list<CCarObject*> dieObject{};

	CCubeMeshDiffused* enemyBoxMesh{ nullptr };
	CCubeMeshDiffused* particleBoxMesh{ nullptr };
	const int maxEnemyBoxCount{ 10 };

};

