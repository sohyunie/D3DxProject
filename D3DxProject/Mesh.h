#pragma once

// ������ ǥ���ϱ� ���� Ŭ������ �����Ѵ�.
class CVertex
{
public:
	CVertex();
	CVertex(XMFLOAT3 xmf3Position);
	~CVertex();

protected:
	// ������ ��ġ �����̴�.(��� ������ �ּ��� ��ġ ���͸� ������ �Ѵ�)
	XMFLOAT3 m_xmf3Position{};
};

class CDiffusedVertex : public CVertex
{
public:
	CDiffusedVertex();
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse);
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse);
	~CDiffusedVertex();

protected:
	// ������ �����̴�.
	XMFLOAT4 m_xmf4Diffuse{};
};

class CMesh
{
public:
	CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CMesh();

private:
	int m_nReferences{};

protected:
	ID3D12Resource* m_pd3dVertexBuffer{};
	ID3D12Resource* m_pd3dVertexUploadBuffer{};
	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView{};
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	UINT m_nSlot{};
	UINT m_nVertices{};
	UINT m_nStride{};
	UINT m_nOffset{};

protected:
	ID3D12Resource* m_pd3dIndexBuffer{};
	ID3D12Resource* m_pd3dIndexUploadBuffer{};
	XMFLOAT3* m_pxmf3Positions = NULL;
	ID3D12Resource* m_pd3dPositionBuffer = NULL;
	ID3D12Resource* m_pd3dPositionUploadBuffer = NULL;
	UINT							m_nVertexBufferViews = 0;
	// �ε��� ����(�ε����� �迭)�� �ε��� ���۸� ���� ���ε� ���ۿ� ���� �������̽� �������̴�.
	// �ε��� ���۴� ��������(�迭)�� ���� �ε����� ������.
	D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView{};
	XMFLOAT2* m_pxmf2TextureCoords = NULL;
	ID3D12Resource* m_pd3dTextureCoordBuffer = NULL;
	ID3D12Resource* m_pd3dTextureCoordUploadBuffer = NULL;
	XMFLOAT3* m_pxmf3Normals = NULL;
	ID3D12Resource* m_pd3dNormalBuffer = NULL;
	ID3D12Resource* m_pd3dNormalUploadBuffer = NULL;
	// �ε��� ���ۿ� ���ԵǴ� �ε����� �����̴�.
	UINT m_nIndices{};
	UINT* m_pnIndices = NULL;
	// �ε��� ���ۿ��� �޽��� �׸��� ���� ���Ǵ� ���� �ε����̴�.
	UINT m_nStartIndex{};

	// �ε��� ������ �ε����� ������ �ε����̴�.
	int m_nBaseVertex{};

public:
	void AddRef();
	void Release();
	void ReleaseUploadBuffers();

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, char* pstrFileName, bool bTextFile);
public:
	BoundingOrientedBox			m_xmOOBB;
};


class CCubeMeshDiffused : public CMesh
{
public:
	// ������ü�� ����, ����, ������ ���̸� �����Ͽ� ������ü �޽��� �����Ѵ�.
	CCubeMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshDiffused();
};

class CHeightMapImage
{
private:
	//���� �� �̹��� �ȼ�(8-��Ʈ)���� ������ �迭�̴�. �� �ȼ��� 0~255�� ���� ���´�.
	BYTE* m_pHeightMapPixels;
	//���� �� �̹����� ���ο� ���� ũ���̴�.
	int m_nWidth;
	int m_nLength;
	//���� �� �̹����� ������ �� �� Ȯ���Ͽ� ����� ���ΰ��� ��Ÿ���� ������ �����̴�.
	XMFLOAT3 m_xmf3Scale;
public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	~CHeightMapImage(void);
	//���� �� �̹������� (x, z) ��ġ�� �ȼ� ���� ����� ������ ���̸� ��ȯ�Ѵ�.
	float GetHeight(float x, float z);
	//���� �� �̹������� (x, z) ��ġ�� ���� ���͸� ��ȯ�Ѵ�.
	XMFLOAT3 GetHeightMapNormal(int x, int z);
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	BYTE* GetHeightMapPixels() { return(m_pHeightMapPixels); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

class CHeightMapGridMesh : public CMesh
{
protected:
	//������ ũ��(����: x-����, ����: z-����)�̴�.
	int m_nWidth;
	int m_nLength;
	/*������ ������(����: x-����, ����: z-����, ����: y-����) �����̴�. ���� ���� �޽��� �� ������ x-��ǥ, y-��ǥ, z-��ǥ�� ������ ������ x-��ǥ, y-��ǥ, z-��ǥ�� ���� ���� ���´�. ��, ���� ������ x-�� ������ ������ 1�� �ƴ�
	�� ������ ������ x-��ǥ�� �ȴ�. �̷��� �ϸ� ���� ����(���� ����)�� ����ϴ��� ū ũ���� ����(����)�� ������
	�� �ִ�.*/
	XMFLOAT3 m_xmf3Scale;
public:
	CHeightMapGridMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale =
		XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void
		* pContext = NULL);
	virtual ~CHeightMapGridMesh();
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }
	//������ ��ǥ�� (x, z)�� �� ����(����)�� ���̸� ��ȯ�ϴ� �Լ��̴�.
	virtual float OnGetHeight(int x, int z, void* pContext);
	//������ ��ǥ�� (x, z)�� �� ����(����)�� ������ ��ȯ�ϴ� �Լ��̴�.
	virtual XMFLOAT4 OnGetColor(int x, int z, void* pContext);
};
