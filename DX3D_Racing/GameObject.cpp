#pragma once

#include "GameObject.h"
#include "Shader.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CShader* CMaterial::m_pPseudoLightingShader = NULL;

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
	if (m_pShader) m_pShader->Release();
}

void CMaterial::SetShader(CShader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CMaterial::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CMaterial::PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	/*m_pPseudoLightingShader = new CPseudoLightingShader();
	m_pPseudoLightingShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pPseudoLightingShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CGameObject::CGameObject(int nMeshes)
{
	m_xmf4x4Transform	= Matrix4x4::Identity();
	m_xmf4x4World		= Matrix4x4::Identity();
	m_nMeshes = nMeshes;
	m_ppMeshes = NULL;
	if (m_nMeshes > 0)
	{
		m_ppMeshes = new CMesh * [m_nMeshes];
		for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;
	}
}

CGameObject::~CGameObject()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}

void CGameObject::CreateShaderVariables(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameObject::ReleaseShaderVariables()
{
}

void CGameObject::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void CGameObject::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, CMaterial* pMaterial)
{
}

void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
	//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�. 
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);
	
	// if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
	if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
	
	//���� ��ü�� �����ϴ� ��� �޽��� �������Ѵ�. 
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) {
				m_ppMeshes[i]->Render(pd3dCommandList);
			}
		}
	}

	if (m_nMaterials > 0)
	{

		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i])
			{
				if (m_ppMaterials[i]->m_pShader) m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
				m_ppMaterials[i]->UpdateShaderVariable(pd3dCommandList);
			}

			if (m_pMesh) m_pMesh->Render(pd3dCommandList, i);
		}
	}
	if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera);
	if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera);
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;
	m_xmf4x4Transform._43 = z;

	/*m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;*/

	UpdateTransform(NULL);
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CGameObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxScale, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

//���� ��ü�� ���� z-�� ���͸� ��ȯ�Ѵ�. 
XMFLOAT3 CGameObject::GetLook()
{
	XMFLOAT3 a = { m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33 };

	return(Vector3::Normalize(a));
}

//���� ��ü�� ���� y-�� ���͸� ��ȯ�Ѵ�.
XMFLOAT3 CGameObject::GetUp()
{
	// return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23 )));

	XMFLOAT3 a = { m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23 };
	 
	return(Vector3::Normalize(a));
}

//���� ��ü�� ���� x-�� ���͸� ��ȯ�Ѵ�. 
XMFLOAT3 CGameObject::GetRight()
{
	// return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));

	XMFLOAT3 a = { m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13 };

	return(Vector3::Normalize(a));
}

//���� ��ü�� ���� x-�� �������� �̵��Ѵ�. 
void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

//���� ��ü�� ���� y-�� �������� �̵��Ѵ�. 
void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//���� ��ü�� ���� z-�� �������� �̵��Ѵ�. 
void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//���� ��ü�� �־��� ������ ȸ���Ѵ�. 
void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	// m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

void CGameObject::Rotate(XMFLOAT3* pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);

	UpdateTransform(NULL);
}

void CGameObject::Rotate(XMFLOAT4* pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define _WITH_DEBUG_FRAME_HIERARCHY

CMeshLoadInfo* CGameObject::LoadMeshInfoFromFile(wifstream& InFile)
{
	TCHAR pstrToken[256] = { 0 };
	int nPositions = 0, nColors = 0, nNormals = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	CMeshLoadInfo* pMeshInfo = new CMeshLoadInfo;

	InFile >> pMeshInfo->m_nVertices;
	InFile >> pMeshInfo->m_pstrMeshName;

	for ( ; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Bounds>:")))
		{
			InFile >> pMeshInfo->m_xmf3AABBCenter.x >> pMeshInfo->m_xmf3AABBCenter.y >> pMeshInfo->m_xmf3AABBCenter.z >> pMeshInfo->m_xmf3AABBExtents.x >> pMeshInfo->m_xmf3AABBExtents.y >> pMeshInfo->m_xmf3AABBExtents.z;
		}
		else if (!_tcscmp(pstrToken, _T("<Positions>:")))
		{
			InFile >> nPositions;
			if (nPositions > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_POSITION;
				pMeshInfo->m_pxmf3Positions = new XMFLOAT3[nPositions];
				for (int i = 0; i < nPositions; i++)
				{
					InFile >> pMeshInfo->m_pxmf3Positions[i].x >> pMeshInfo->m_pxmf3Positions[i].y >> pMeshInfo->m_pxmf3Positions[i].z;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Colors>:")))
		{
			InFile >> nColors;
			if (nColors > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_COLOR;
				pMeshInfo->m_pxmf4Colors = new XMFLOAT4[nColors];
				for (int i = 0; i < nColors; i++)
				{
					InFile >> pMeshInfo->m_pxmf4Colors[i].x >> pMeshInfo->m_pxmf4Colors[i].y >> pMeshInfo->m_pxmf4Colors[i].z >> pMeshInfo->m_pxmf4Colors[i].w;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Normals>:")))
		{
			InFile >> nNormals;
			if (nNormals > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_NORMAL;
				pMeshInfo->m_pxmf3Normals = new XMFLOAT3[nNormals];
				for (int i = 0; i < nNormals; i++)
				{
					InFile >> pMeshInfo->m_pxmf3Normals[i].x >> pMeshInfo->m_pxmf3Normals[i].y >> pMeshInfo->m_pxmf3Normals[i].z;
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Indices>:")))
		{
			InFile >> nIndices;
			if (nIndices > 0)
			{
				pMeshInfo->m_pnIndices = new UINT[nIndices];
				for (int i = 0; i < nIndices; i++) InFile >> pMeshInfo->m_pnIndices[i];
			}
		}
		else if (!_tcscmp(pstrToken, _T("<SubMeshes>:")))
		{
			InFile >> pMeshInfo->m_nSubMeshes;
			if (pMeshInfo->m_nSubMeshes > 0)
			{
				pMeshInfo->m_pnSubSetIndices = new int[pMeshInfo->m_nSubMeshes];
				pMeshInfo->m_ppnSubSetIndices = new UINT * [pMeshInfo->m_nSubMeshes];
				for (int i = 0; i < pMeshInfo->m_nSubMeshes; i++)
				{
					InFile >> pstrToken;
					if (!_tcscmp(pstrToken, _T("<SubMesh>:")))
					{
						InFile >> pstrToken; //i
						InFile >> pMeshInfo->m_pnSubSetIndices[i];
						if (pMeshInfo->m_pnSubSetIndices[i] > 0)
						{
							pMeshInfo->m_ppnSubSetIndices[i] = new UINT[pMeshInfo->m_pnSubSetIndices[i]];
							for (int j = 0; j < pMeshInfo->m_pnSubSetIndices[i]; j++) InFile >> pMeshInfo->m_ppnSubSetIndices[i][j];
						}

					}
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("</Mesh>")))
		{
			break;
		}
	}
	return(pMeshInfo);
}

MATERIALSLOADINFO* CGameObject::LoadMaterialsInfoFromFile(wifstream& InFile)
{
	TCHAR pstrToken[256] = { 0 };
	int nMaterial = 0;

	MATERIALSLOADINFO* pMaterialsInfo = new MATERIALSLOADINFO;


	InFile >> pMaterialsInfo->m_nMaterials;
	pMaterialsInfo->m_pMaterials = new MATERIALLOADINFO[pMaterialsInfo->m_nMaterials];


	for (; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Material>:")))
		{
			InFile >> nMaterial;
		}
		else if (!_tcscmp(pstrToken, _T("<AlbedoColor>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.x >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.y >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.z >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor.w;
		}
		else if (!_tcscmp(pstrToken, _T("<EmissiveColor>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.x >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.y >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.z >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor.w;
		}
		else if (!_tcscmp(pstrToken, _T("<SpecularColor>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.x >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.y >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.z >> pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor.w;
		}
		else if (!_tcscmp(pstrToken, _T("<Glossiness>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossiness;
		}
		else if (!_tcscmp(pstrToken, _T("<Smoothness>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fSmoothness;
		}
		else if (!_tcscmp(pstrToken, _T("<Metallic>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fSpecularHighlight;
		}
		else if (!_tcscmp(pstrToken, _T("<SpecularHighlight>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fMetallic;
		}
		else if (!_tcscmp(pstrToken, _T("<GlossyReflection>:")))
		{
			InFile >> pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossyReflection;
		}
		else if (!_tcscmp(pstrToken, _T("</Materials>")))
		{
			break;
		}
	}
	return(pMaterialsInfo);
}

CGameObject* CGameObject::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, wifstream& InFile)
{
	TCHAR pstrToken[256] = { 0 };

	int nFrame = 0;

	CGameObject* pGameObject = NULL;

	while(true)
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Frame>:")))
		{
			pGameObject = new CGameObject();

			InFile >> nFrame;
			InFile >> pGameObject->m_pstrFrameName;
		}
		else if (!_tcscmp(pstrToken, _T("<Transform>:")))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			InFile >> xmf3Position.x >> xmf3Position.y >> xmf3Position.z;
			InFile >> xmf3Rotation.x >> xmf3Rotation.y >> xmf3Rotation.z; //Euler Angle
			InFile >> xmf3Scale.x >> xmf3Scale.y >> xmf3Scale.z;
			InFile >> xmf4Rotation.x >> xmf4Rotation.y >> xmf4Rotation.z >> xmf4Rotation.w; //Quaternion
		}
		else if (!_tcscmp(pstrToken, _T("<TransformMatrix>:")))
		{
			InFile >> pGameObject->m_xmf4x4Transform._11 >> pGameObject->m_xmf4x4Transform._12 >> pGameObject->m_xmf4x4Transform._13 >> pGameObject->m_xmf4x4Transform._14; //ToParentTransform
			InFile >> pGameObject->m_xmf4x4Transform._21 >> pGameObject->m_xmf4x4Transform._22 >> pGameObject->m_xmf4x4Transform._23 >> pGameObject->m_xmf4x4Transform._24;
			InFile >> pGameObject->m_xmf4x4Transform._31 >> pGameObject->m_xmf4x4Transform._32 >> pGameObject->m_xmf4x4Transform._33 >> pGameObject->m_xmf4x4Transform._34;
			InFile >> pGameObject->m_xmf4x4Transform._41 >> pGameObject->m_xmf4x4Transform._42 >> pGameObject->m_xmf4x4Transform._43 >> pGameObject->m_xmf4x4Transform._44;
		}
		else if (!_tcscmp(pstrToken, _T("<Mesh>:")))
		{
			CMeshLoadInfo* pMeshInfo = pGameObject->LoadMeshInfoFromFile(InFile);
			if (pMeshInfo)
			{
				CMesh* pMesh = NULL;
				if (pMeshInfo->m_nType & VERTEXT_NORMAL)
				{
					pMesh = new CMeshIlluminatedFromFile(pd3dDevice, pd3dCommandList, pMeshInfo);
				}
				if (pMesh) pGameObject->SetMesh(pMesh);
				delete pMeshInfo;
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Materials>:")))
		{
			MATERIALSLOADINFO* pMaterialsInfo = pGameObject->LoadMaterialsInfoFromFile(InFile);
			if (pMaterialsInfo && (pMaterialsInfo->m_nMaterials > 0))
			{
				pGameObject->m_nMaterials = pMaterialsInfo->m_nMaterials;

				pGameObject->m_ppMaterials = new CMaterial * [pMaterialsInfo->m_nMaterials];


				for (int i = 0; i < pMaterialsInfo->m_nMaterials; i++)
				{
					pGameObject->m_ppMaterials[i] = NULL;	// �ʱ�ȭ

					CMaterial* pMaterial = new CMaterial();
					if (pGameObject->GetMeshType() & VERTEXT_NORMAL)
					{
						pMaterial->SetPseudoLightingShader();
					}

					pGameObject->SetMaterial(i, pMaterial);
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("<Children>:")))
		{
			int nChilds = 0;
			InFile >> nChilds;

			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					CGameObject* pChild = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, InFile);
					if (pChild) pGameObject->SetChild(pChild);
#ifdef _WITH_DEBUG_RUNTIME_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, _T("(Child Frame: %p) (Parent Frame: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!_tcscmp(pstrToken, _T("</Frame>")))
		{
			break;
		}
	}

	return(pGameObject);
}

void CGameObject::PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	if (pGameObject->m_pSibling) CGameObject::PrintFrameInfo(pGameObject->m_pSibling, pParent);
	if (pGameObject->m_pChild) CGameObject::PrintFrameInfo(pGameObject->m_pChild, pGameObject);
}

CGameObject* CGameObject::LoadGeometryFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, TCHAR* pstrFileName)
{
	CGameObject* pGameObject = NULL;

	wifstream InFile(pstrFileName);

	TCHAR pstrToken[256] = { 0 };

	for (; ; )
	{
		InFile >> pstrToken;
		if (!InFile) break;

		if (!_tcscmp(pstrToken, _T("<Hierarchy>:")))
		{
			pGameObject = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, InFile);
		}
		else if (!_tcscmp(pstrToken, _T("</Hierarchy>")))
		{
			break;
		}
	}

//#ifdef _WITH_DEBUG_FRAME_HIERARCHY
//	TCHAR pstrDebug[256] = { 0 };
//	_stprintf_s(pstrDebug, 256, _T("\nFrame Hierarchy(%s)\n"), pstrFileName);
//	OutputDebugString(pstrDebug);
//
//	CGameObject::PrintFrameInfo(pGameObject, NULL);
//#endif

	return(pGameObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//

CRotatingObject::CRotatingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}

void CRotatingObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}

void CGameObject::UpdateTransform(XMFLOAT4X4* pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4Transform, *pxmf4x4Parent) : m_xmf4x4Transform;

	if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

CGameObject* CGameObject::FindFrame(_TCHAR* pstrFrameName)
{
	CGameObject* pFrameObject = NULL;
	if (!_tcsncmp(m_pstrFrameName, pstrFrameName, _tcslen(pstrFrameName))) return(this);

	if (m_pSibling) if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) return(pFrameObject);
	if (m_pChild) if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) return(pFrameObject);

	return(NULL);
}

void CGameObject::SetShader(CShader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();

	m_nMaterials = 1;
	m_ppMaterials = new CMaterial * [m_nMaterials];
	m_ppMaterials[0] = new CMaterial();
	m_ppMaterials[0]->SetShader(pShader);
}

void CGameObject::SetShader(int nMaterial, CShader* pShader)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->SetShader(pShader);
}

void CGameObject::SetMaterial(int nMaterial, CMaterial* pMaterial)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->Release();
	m_ppMaterials[nMaterial] = pMaterial;
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->AddRef();
}

void CGameObject::SetChild(CGameObject* pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}

	if (pChild) pChild->m_pParent = this;
}

void CGameObject::SetMesh(int nIndex, CMesh* pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void CGameObject::SetMesh(CMesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CGameObject::ReleaseUploadBuffers()
{
	/*if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->ReleaseUploadBuffers();
		}
	}*/

	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();

	if (m_pSibling) m_pSibling->ReleaseUploadBuffers();
	if (m_pChild) m_pChild->ReleaseUploadBuffers();
}

void CGameObject::Animate(float fTimeElapsed)
{
}

void CGameObject::OnPrepareRender()
{
}

CHeightMapTerrain::CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int
	nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4
	xmf4Color) : CGameObject(0)
{
	//������ ����� ���� ���� ����, ������ ũ���̴�. 
	m_nWidth = nWidth;
	m_nLength = nLength;

	/*���� ��ü�� ���� �޽����� �迭�� ���� ���̴�. nBlockWidth, nBlockLength�� ���� �޽� �ϳ��� ����, ���� ũ
	���̴�. cxQuadsPerBlock, czQuadsPerBlock�� ���� �޽��� ���� ����� ���� ���� �簢���� �����̴�.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	//xmf3Scale�� ������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ����. 
	m_xmf3Scale = xmf3Scale;

	//������ ����� ���� ���� �����Ѵ�. 
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	//�������� ���� ����, ���� �������� ���� �޽��� �� ���� �ִ� ���� ��Ÿ����. 
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	//���� ��ü�� ǥ���ϱ� ���� ���� �޽��� �����̴�. 
	m_nMeshes = cxBlocks * czBlocks;

	//���� ��ü�� ǥ���ϱ� ���� ���� �޽��� ���� ������ �迭�� �����Ѵ�. 
	m_ppMeshes = new CMesh * [m_nMeshes];

	for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;

	CHeightMapGridMesh* pHeightMapGridMesh = NULL;

	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//������ �Ϻκ��� ��Ÿ���� ���� �޽��� ���� ��ġ(��ǥ)�̴�. 
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);

			//������ �Ϻκ��� ��Ÿ���� ���� �޽��� �����Ͽ� ���� �޽��� �����Ѵ�. 
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z * cxBlocks), pHeightMapGridMesh);
		}
	}

	//������ �������ϱ� ���� ���̴��� �����Ѵ�. 
	CTerrainShader* pShader = new CTerrainShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	SetShader(pShader);
}

CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CApacheObject::CApacheObject()
{
}

CApacheObject::~CApacheObject()
{
}

void CApacheObject::OnInitialize()
{
	m_pMainRotorFrame0 = FindFrame((WCHAR*)L"rotor");
	//	m_pMainRotorFrame1 = FindFrame(L"black_m_6");
	m_pTailRotorFrame0 = FindFrame((WCHAR*)L"black_m_7");
	//	m_pTailRotorFrame1 = FindFrame(L"black_m_8");
}

void CApacheObject::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	if (m_pMainRotorFrame0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 2.0f) * fTimeElapsed);
		m_pMainRotorFrame0->m_xmf4x4Transform = Matrix4x4::Multiply(xmmtxRotate, m_pMainRotorFrame0->m_xmf4x4Transform);
	}
	if (m_pTailRotorFrame0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 4.0f) * fTimeElapsed);
		m_pTailRotorFrame0->m_xmf4x4Transform = Matrix4x4::Multiply(xmmtxRotate, m_pTailRotorFrame0->m_xmf4x4Transform);
	}

	//CGameObject::Animate(fTimeElapsed, pxmf4x4Parent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//