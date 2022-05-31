#pragma once
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{

}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

void CScene::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CScene::ReleaseShaderVariables()
{
}

#define _WITH_TERRAIN_PARTITION

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_nGameObjects = 360;	// 총 나무 개수
	m_nGameObjects += 12;	// 달리는 차량
	m_nGameObjects += 4;	// 공중 헬기
	m_ppGameObjects = new CGameObject * [m_nGameObjects];

	int tree_number = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 가로줄 나무 심기
	for (int line = 0; line < 2; ++line) {
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 45; ++j) {
				CGameObject* Tree_1 = new CGameObject();
				CGameObject* pTree_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/Tree.txt");
				Tree_1->OnInitialize();
				Tree_1->SetChild(pTree_1);
				Tree_1->SetScale(8.0f, 8.0f, 8.0f);
				Tree_1->SetPosition(410.0f + j * 100.0f, 0.0f, 200.0f * i + line * 4920);

				XMFLOAT3 a = { 410.0f + j * 100.0f, 0.0f, 200.0f * i + line * 4920 };
				XMFLOAT3 b = { 100.0f, 100.0f, 25.0f };

				Tree_1->OOBB.Center = a;
				Tree_1->OOBB.Extents = b;

				m_ppGameObjects[tree_number++] = Tree_1;
			}
		}
	}

	// 세로줄 나무 심기
	for (int line = 0; line < 2; ++line) {
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 45; ++j) {
				CGameObject* Tree_1 = new CGameObject();
				CGameObject* pTree_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/Tree.txt");
				Tree_1->OnInitialize();
				Tree_1->SetChild(pTree_1);
				Tree_1->SetScale(8.0f, 8.0f, 8.0f);

				Tree_1->SetPosition(200.0f * i + line * 4920, 0.0f, 410.0f + j * 100.0f);

				XMFLOAT3 a = { 200.0f * i + line * 4920, 0.0f, 410.0f + j * 100.0f };
				XMFLOAT3 b = { 25.0f, 100.0f, 100.0f };

				Tree_1->OOBB.Center = a;
				Tree_1->OOBB.Extents = b;

				m_ppGameObjects[tree_number++] = Tree_1;
			}
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 0 가로 이동 차량 3대
	for (int i = 0; i < 3; ++i) {
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/PoliceCar.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(6.0f, 6.0f, 6.0f);
		Car_1->Rotate(0.0f, 90.0f, 0.0f);
		Car_1->SetPosition(40.0f , 1.0f, 40.0f + i * 40.0f);

		XMFLOAT3 a = { 40.0f, 1.0f, 40.0f + i * 40.0f };
		XMFLOAT3 b = { 40.0f, 50.0f, 20.0f };
		Car_1->OOBB.Center = a;
		Car_1->OOBB.Extents = b;

		m_ppGameObjects[tree_number++] = Car_1;
	}

	// +Z 가로 이동 차량 3대
	for (int i = 0; i < 3; ++i) {
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/PoliceCar.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(6.0f, 6.0f, 6.0f);
		Car_1->Rotate(0.0f, -90.0f, 0.0f);
		Car_1->SetPosition(5000.0f, 1.0f, 40.0f + i * 40.0f + 4950.0f);

		XMFLOAT3 a = { 5000.0f, 1.0f, 40.0f + i * 40.0f + 4950.0f };
		XMFLOAT3 b = { 40.0f, 50.0f, 20.0f };
		Car_1->OOBB.Center = a;
		Car_1->OOBB.Extents = b;

		m_ppGameObjects[tree_number++] = Car_1;
	}
	
	// 0 세로 이동 차량 3대
	for (int i = 0; i < 3; ++i) {
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/PoliceCar.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(6.0f, 6.0f, 6.0f);
		Car_1->Rotate(0.0f, 180.0f, 0.0f);
		Car_1->SetPosition(40.0f + i * 40.0f, 1.0f, 5000.0f);

		XMFLOAT3 a = { 40.0f + i * 40.0f, 1.0f, 5000.0f };
		XMFLOAT3 b = { 20.0f, 50.0f, 40.0f };
		Car_1->OOBB.Center = a;
		Car_1->OOBB.Extents = b;

		m_ppGameObjects[tree_number++] = Car_1;
	}

	// +X 세로 이동 차량 3대
	for (int i = 0; i < 3; ++i) {
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/PoliceCar.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(6.0f, 6.0f, 6.0f);
		Car_1->SetPosition(i * 40.0f + 4950.0f, 1.0f, 20.0f);

		XMFLOAT3 a = { i * 40.0f + 4950.0f, 1.0f, 20.0f };
		XMFLOAT3 b = { 20.0f, 50.0f, 40.0f };
		Car_1->OOBB.Center = a;
		Car_1->OOBB.Extents = b;

		m_ppGameObjects[tree_number++] = Car_1;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	{
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/Apache.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(1.5f, 1.5f, 1.5f);
		Car_1->Rotate(0.0f, -90.0f, 0.0f);
		Car_1->SetPosition(2500.0f, 100.0f, 70.0f);

		m_ppGameObjects[tree_number++] = Car_1;
	}
	{
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/Apache.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(1.5f, 1.5f, 1.5f);
		Car_1->Rotate(0.0f, 0.0f, 0.0f);
		Car_1->SetPosition(70.0f, 100.0f, 2500.0f);

		m_ppGameObjects[tree_number++] = Car_1;
	}
	{
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/Apache.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(1.5f, 1.5f, 1.5f);
		Car_1->Rotate(0.0f, 180.0f, 0.0f);
		Car_1->SetPosition(4950.0f, 100.0f, 2500.0f);

		m_ppGameObjects[tree_number++] = Car_1;
	}
	{
		CGameObject* Car_1 = new CGameObject();
		CGameObject* pCar_1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, (WCHAR*)L"Assets/Model/Apache.txt");
		Car_1->OnInitialize();
		Car_1->SetChild(pCar_1);
		Car_1->SetScale(1.5f, 1.5f, 1.5f);
		Car_1->Rotate(0.0f, 90.0f, 0.0f);
		Car_1->SetPosition(2500.0f, 100.0f, 4950.0f);

		m_ppGameObjects[tree_number++] = Car_1;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	XMFLOAT3 xmf3Scale(20.0f, 0.01f, 20.0f);
	XMFLOAT4 xmf4Color(0.48f, 0.3f, 0.15f, 0.0f);

	//지형을 높이 맵 이미지 파일(HeightMap.raw)을 사용하여 생성한다. 높이 맵의 크기는 가로x세로(257x257)이다. 
#ifdef _WITH_TERRAIN_PARTITION
	
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/heightmap01.raw"), 257, 257, 17, 17, xmf3Scale, xmf4Color);
#else
	//지형을 하나의 격자 메쉬(257x257)로 생성한다. 
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/heightmap01.raw"), 257, 257, 257, 257, xmf3Scale, xmf4Color);
#endif

	// Terrain에 포함된 child object들을 출력합니다.
	m_nShaders = 1;
	m_pShaders = new CObjectsShader[m_nShaders];
	m_pShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pShaders[0].BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].ReleaseShaderVariables();
		m_pShaders[i].ReleaseObjects();
	}
	if (m_pShaders) delete[] m_pShaders;
	if (m_pTerrain) delete m_pTerrain;

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) delete m_ppGameObjects[i];
		delete[] m_ppGameObjects;
	}
}

bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return false;
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].AnimateObjects(fTimeElapsed);
	}

	for (int i = 0; i < 360; ++i) {
		ContainmentType collision_type = m_pPlayer->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (collision_type)
		{
		case DirectX::DISJOINT:
			// m_pPlayer->collision_is = false;
			break;
		case DirectX::INTERSECTS:
			m_ppGameObjects[i]->SetScale(0.1f, 0.1f, 0.1f);
			m_pPlayer->collision_is = true;
			break;
		case DirectX::CONTAINS:
			m_pPlayer->SetPosition(m_pPlayer->pPrev_Position);
			break;
		default:
			break;
		}
	}

	for (int i = 360; i < 363; i++) {
		ContainmentType collision_type = m_pPlayer->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (collision_type)
		{
		case DirectX::DISJOINT:
			m_ppGameObjects[i]->SetPosition(m_ppGameObjects[i]->GetPosition().x + (i * i / 1000) * fTimeElapsed, m_ppGameObjects[i]->GetPosition().y, m_ppGameObjects[i]->GetPosition().z);
			if (m_ppGameObjects[i]->GetPosition().x > 5000.0f)
				m_ppGameObjects[i]->SetPosition(40.0f, 1.0f, 40.0f + (i - 360) * 40.0f);
			break;
		case DirectX::INTERSECTS:
			m_ppGameObjects[i]->SetPosition(40.0f, 1.0f, 40.0f + (i - 360) * 40.0f);
			break;
		case DirectX::CONTAINS:
			m_ppGameObjects[i]->SetPosition(40.0f, 1.0f, 40.0f + (i - 360) * 40.0f);
			break;
		default:
			break;
		}
		m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();
	}
	for (int i = 363; i < 366; ++i) {
		ContainmentType collision_type = m_pPlayer->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (collision_type)
		{
		case DirectX::DISJOINT:
			m_ppGameObjects[i]->SetPosition(m_ppGameObjects[i]->GetPosition().x - (i * i / 1000) * fTimeElapsed, m_ppGameObjects[i]->GetPosition().y, m_ppGameObjects[i]->GetPosition().z);
			if (m_ppGameObjects[i]->GetPosition().x < 0.0f)
				m_ppGameObjects[i]->SetPosition(5000.0f, 1.0f, (i - 363) * 40.0f + 4950.0f);
			break;
		case DirectX::INTERSECTS:
			m_ppGameObjects[i]->SetPosition(5000.0f, 1.0f, (i - 363) * 40.0f + 4950.0f);
			break;
		case DirectX::CONTAINS:
			m_ppGameObjects[i]->SetPosition(5000.0f, 1.0f, (i - 363) * 40.0f + 4950.0f);
			break;
		default:
			break;
		}
		m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();
	}
	for (int i = 366; i < 369; ++i) {
		ContainmentType collision_type = m_pPlayer->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (collision_type)
		{
		case DirectX::DISJOINT:
			m_ppGameObjects[i]->SetPosition(m_ppGameObjects[i]->GetPosition().x, m_ppGameObjects[i]->GetPosition().y, m_ppGameObjects[i]->GetPosition().z - (i * i / 1000) * fTimeElapsed);
			if (m_ppGameObjects[i]->GetPosition().z < 0.0f)
				m_ppGameObjects[i]->SetPosition(40.0f + (i - 366) * 40.0f, 1.0f, 5000.0f);
			break;
		case DirectX::INTERSECTS:
			m_ppGameObjects[i]->SetPosition(40.0f + (i - 366) * 40.0f, 1.0f, 5000.0f);
			break;
		case DirectX::CONTAINS:
			m_ppGameObjects[i]->SetPosition(40.0f + (i - 366) * 40.0f, 1.0f, 5000.0f);
			break;
		default:
			break;
		}
		m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();
	}
	for (int i = 369; i < 372; ++i) {
		ContainmentType collision_type = m_pPlayer->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (collision_type)
		{
		case DirectX::DISJOINT:
			m_ppGameObjects[i]->SetPosition(m_ppGameObjects[i]->GetPosition().x, m_ppGameObjects[i]->GetPosition().y, m_ppGameObjects[i]->GetPosition().z + (i * i / 1000) * fTimeElapsed);
			if (m_ppGameObjects[i]->GetPosition().z > 5000.0f)
				m_ppGameObjects[i]->SetPosition(40.0f + (i - 369) * 40.0f + 4950.0f, 1.0f, 20.0f);
			break;
		case DirectX::INTERSECTS:
			m_ppGameObjects[i]->SetPosition(40.0f + (i - 369) * 40.0f + 4950.0f, 1.0f, 20.0f);
			break;
		case DirectX::CONTAINS:
			m_ppGameObjects[i]->SetPosition(40.0f + (i - 369) * 40.0f + 4950.0f, 1.0f, 20.0f);
			break;
		default:
			break;
		}
		m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();
	}
	
	for (int i = 0; i < m_pShaders[0].m_nObjects; i++) {
		ContainmentType collision_type = m_pPlayer->OOBB.Contains(m_pShaders[0].m_ppObjects[i]->OOBB);
		switch (collision_type)
		{
		case DirectX::DISJOINT:
			// m_pPlayer->collision_is = false;
			break;
		case DirectX::INTERSECTS:
			m_ppGameObjects[i]->SetScale(0.5f, 0.5f, 0.5f);
			m_pPlayer->collision_is = true;
			break;
		case DirectX::CONTAINS:
			m_pPlayer->SetPosition(m_pPlayer->pPrev_Position);
			break;
		default:
			break;
		}

	}
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	if(m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].Render(pd3dCommandList, pCamera);
	}

	for (int i = 0; i < m_nGameObjects; i++) {
		m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseUploadBuffers();
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
}

ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; // Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	
	pd3dRootParameters[0].Constants.Num32BitValues = 16;
	pd3dRootParameters[0].Constants.ShaderRegister = 0;
	pd3dRootParameters[0].Constants.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;


	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 1;	// 2로 바꾸면 오류??
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =	D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
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
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, 
									pd3dSignatureBlob->GetBufferPointer(),
									pd3dSignatureBlob->GetBufferSize(),
									__uuidof(ID3D12RootSignature), 
									(void**)&pd3dGraphicsRootSignature);

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
	return(pd3dGraphicsRootSignature);
}

ID3D12RootSignature* CScene::GetGraphicsRootSignature()
{
	return(m_pd3dGraphicsRootSignature);
}