#pragma once
#include "Player.h"

#define MAX_LIGHTS			1
#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

class CShader;
class CMirrorShader;

struct Light
{
	bool								m_IsActive{};

	XMFLOAT3							m_Position{};
	XMFLOAT3							m_Direction{};

	int									m_Type{};

	XMFLOAT4							m_Diffuse{};
	XMFLOAT4							m_Ambient{};
	XMFLOAT4							m_Specular{};

	XMFLOAT3							m_Attenuation;
	float 								m_Falloff{};
	float								m_Range;
	float 								m_Theta{};
	float								m_Phi{};

	float								PADDING{};
};

struct CB_LIGHT
{
	Light								m_Lights[MAX_LIGHTS]{};
	
	XMFLOAT4							m_GlobalAmbient{};
};

// =============================================== CScene ===============================================

class CScene
{
public:
	static queue<unique_ptr<CScene>>	m_Scenes;

protected:
	ComPtr<ID3D12RootSignature>			m_D3D12RootSignature{};

public:
	CScene() = default;
	virtual ~CScene() = default;

	virtual void OnCreate(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
	virtual void OnDestroy() = 0;

	virtual void BuildObjects(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
	virtual void ReleaseObjects() = 0;

	virtual void CreateRootSignature(ID3D12Device* D3D12Device) = 0;

	virtual void CreateShaderVariables(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
	virtual void ReleaseShaderVariables() = 0;

	virtual void ReleaseUploadBuffers() = 0;

	virtual void ProcessMouseMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
	virtual void ProcessKeyboardMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
	virtual void ProcessInput(HWND hWnd, float ElapsedTime) = 0;
	
	virtual void Animate(float ElapsedTime) = 0;
	virtual void Render(ID3D12GraphicsCommandList* D3D12GraphicsCommandList) = 0;
};

// =============================================== CTitleScene ===============================================

class CTitleScene : public CScene
{
private:
	shared_ptr<CBackgroundObject>		m_Background{};
	shared_ptr<CButtonObject>			m_Buttons{};

	shared_ptr<CShader>					m_Shader{};

public:
	CTitleScene() = default;
	virtual ~CTitleScene() = default;

	virtual void OnCreate(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void OnDestroy();

	virtual void BuildObjects(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ReleaseObjects();

	virtual void CreateRootSignature(ID3D12Device* D3D12Device);

	virtual void CreateShaderVariables(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void ProcessMouseMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ProcessKeyboardMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ProcessInput(HWND hWnd, float ElapsedTime);

	virtual void Animate(float ElapsedTime);
	virtual void Render(ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
};

// =============================================== CGameScene ===============================================

class CGameScene : public CScene
{
public:
	bool								m_FreeCamera{};

private:
	shared_ptr<CPlayer>					m_Player{};

	vector<shared_ptr<CObject>>			m_Objects{};
	shared_ptr<CObject>					m_SelectedObject{};

	shared_ptr<CTerrainObject>			m_Terrain{};

	vector<shared_ptr<CObject>>			m_Walls{};

	// Bilboard
	shared_ptr<CHpBarObject>			m_HpBars{};
	shared_ptr<CExplodedEnemyObject>	m_ExplodedEnemies{};
	shared_ptr<CSkyBoxObject>			m_SkyBox{};
	shared_ptr<CTreeObject>				m_Trees{};
	shared_ptr<CSmokeObject>			m_Smoke{};

	vector<shared_ptr<CShader>>			m_Shaders{};

	// Mirror
	shared_ptr<CObject>					m_Mirror{};
	shared_ptr<CMirrorShader>			m_MirrorShader{};

	vector<Light>						m_Lights{};
	ComPtr<ID3D12Resource>				m_D3D12LightsConstantBuffer{};
	CB_LIGHT*							m_MappedLights{};
	XMFLOAT4							m_GlobalAmbient{};

public:
	CGameScene() = default;
	virtual ~CGameScene() = default;

	virtual void OnCreate(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void OnDestroy();

	virtual void BuildObjects(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ReleaseObjects();

	virtual void CreateRootSignature(ID3D12Device* D3D12Device);

	virtual void CreateShaderVariables(ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void ProcessMouseMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ProcessKeyboardMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, ID3D12Device* D3D12Device, ID3D12GraphicsCommandList* D3D12GraphicsCommandList);
	virtual void ProcessInput(HWND hWnd, float ElapsedTime);

	virtual void Animate(float ElapsedTime);
	virtual void Render(ID3D12GraphicsCommandList* D3D12GraphicsCommandList);

	void BuildLights();

	void CheckPlayerByEnemyCollision(float ElapsedTime);
	void CheckEnemyByEnemyCollision(float ElapsedTime);
	void CheckBulletByEnemyCollision();
	void CheckBulletByTerrainCollision();

	void PickObjectPointedByCursor(int Xpos, int Ypos);
	const shared_ptr<CObject>& GetSelectedObject();
};
