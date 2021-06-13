#pragma once
#include "Shader.h"
#include "GameObject.h"

class CCarObject : public  CGameObject {
public:
	CCarObject();
	~CCarObject();
	void SetDirection(const XMFLOAT3& dir) {
		XMFLOAT3 tempDir = dir;
		direction = Vector3::Normalize(tempDir);
	}
	virtual void Animate(float elapsedTime, XMFLOAT3 playerPos) override;
	void SetBoom(bool bFlag) {
		isBoom = bFlag;
	}
	bool GetIsBoom() {
		return isBoom;
	}
private:
	XMFLOAT3 direction{};
	BoundingBox boundBox{};
	bool isBoom = false;
};

class CCarObjectParticle : public CGameObject {
public:
	CCarObjectParticle();
	~CCarObjectParticle();

	virtual void Animate(float elapsedTime, XMFLOAT3 playerPos) override;

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
	CCarObject* parent = nullptr;
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

	virtual void AnimateObjects(float elapsedTime, XMFLOAT3 playerPos) override;
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature) override;

protected:

	ID3D12Resource* m_pd3dcbGameObjects = NULL;
	VS_VB_INSTANCE* m_pcbMappedGameObjects = NULL;

	std::vector<CCarObjectParticle*> particles{};
	std::vector<CCarObjectParticle*> particleObject{};
	std::list<CCarObject*> deadObject{};

	CCubeMeshDiffused* enemyBoxMesh = nullptr;
	CCubeMeshDiffused* particleBoxMesh = nullptr;
	const int coinCount = 30;

};

