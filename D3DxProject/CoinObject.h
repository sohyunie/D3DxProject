#pragma once
#include "Shader.h"
#include "GameObject.h"

class CoinObject : public CRotatingObject {
public:
	CoinObject();
	~CoinObject();
	void SetDirection(const XMFLOAT3& dir) {
		XMFLOAT3 tempDir = dir;
		direction = Vector3::Normalize(tempDir);
	}
	virtual void Animate(float elapsedTime, XMFLOAT3 playerPos) override;

	bool isCollision(const BoundingBox& target) {
		BoundingBox curColl{ boundBox };
		curColl.Transform(curColl, XMLoadFloat4x4(&m_xmf4x4World));
		return curColl.Contains(target) != ContainmentType::DISJOINT;
	}
	void SetLive(bool bFlag) {
		isLive = bFlag;
		deletedTime = 0.0f;
	}
	void SetBoom(bool bFlag) {
		isBoom = bFlag;
	}
	bool GetLive() {
		return isLive;
	}
	bool GetIsBoom() {
		return isBoom;
	}
	float GetDeletedTime() {
		return deletedTime;
	}
private:
	XMFLOAT3 direction{};
	BoundingBox boundBox{};
	bool isLive{ true };
	bool isBoom{ false };
	float deletedTime{ 0.0f };
};

class CoinObjectShader : public CInstancingShader {
public:
	CoinObjectShader();
	virtual ~CoinObjectShader();
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

	std::list<CoinObjectShader*> dieObject{};

	CCubeMeshDiffused* coinBoxMesh{ nullptr };
	const int coinCount{ 10 };

};

