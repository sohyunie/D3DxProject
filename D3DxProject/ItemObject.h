#pragma once
#include "Shader.h"
#include "GameObject.h"

class ItemObject : public CRotatingObject {
public:
	ItemObject();
	~ItemObject();
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

class ItemObjectShader : public CInstancingShader {
public:
	ItemObjectShader();
	virtual ~ItemObjectShader();
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

	CCubeMeshDiffused* itemBoxMesh = nullptr;
	const int itemCount = 2;
};
