#pragma once
#include "Scene.h"
#include "Timer.h"
#include "CPlayer.h"
// ���� ���α׷��� ���븦 ��Ÿ����.
// Direct3D Device�� �����ϰ� �����ϸ� ȭ�� ����� ���� ���� ���� ó�� (���� ��ü�� ������ ����, ����� �Է�, �ִϸ��̼� ��)�� ����Ѵ�.
class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd); // �����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ��̴�(�� �����찡 �����Ǹ� ȣ��ȴ�).
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList(); // ���� ü��, ����̽�, ������ ��, ���ť/�Ҵ���/����Ʈ�� �����ϴ� �Լ��̴�.

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView(); // ���� Ÿ�� ��� ����-���ٽ� �並 �����ϴ� �Լ��̴�.

	void ChangeSwapChainState();	// ����ü��

	void BuildObjects();
	void ReleaseObjects(); // �������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�. 

	void ProcessInput();	// �����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ��̴�.
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();		// CUP�� GPU�� ����ȭ�ϴ� �Լ��̴�.

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	// �������� �޼���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�.

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	IDXGIFactory4* m_pdxgiFactory		= NULL;	// DXGI ���丮 �������̽��� ���� �������̴�.
	IDXGISwapChain3* m_pdxgiSwapChain	= NULL;	// ���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ��� �ʿ��ϴ�.
	ID3D12Device* m_pd3dDevice			= NULL;	// Direct3D ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	
	bool m_bMsaa4xEnable				= false;
	UINT m_nMsaa4xQualityLevels			= 0;	// MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ �����Ѵ�.

	static const UINT m_nSwapChainBuffers = 2;	// ����ü���� �ĸ� ������ �����̴�.
	UINT m_nSwapChainBufferIndex;				// ���� ���� ü���� �ĸ� ���� �ε����̴�.

	ID3D12Resource* m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];	// 3D �𵨿� ����ü��
	ID3D12Resource* m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// ���� �޽��� 
	ID3D12DescriptorHeap* m_pd3dRtvDescriptorHeap = NULL;
	UINT m_nRtvDescriptorIncrementSize;									// ���� Ÿ�� ����, ������ �� �������̽� ������, ���� Ÿ�� ������ ������ ũ���̴�.

	ID3D12Resource* m_pd3dDepthStencilBuffer;
	ID3D12DescriptorHeap* m_pd3dDsvDescriptorHeap;
	UINT	m_nDsvDescriptorIncrementSize;								// ����-���ٽ� ����, ������ �� �������̽� ������, ����-���ٽ� ������ ������ ũ���̴�.

	ID3D12CommandAllocator* m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue* m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList* m_pd3dCommandList = NULL;				// ��� ť, ��� �Ҵ���, ��� ����Ʈ �������̽� �������̴�.

	ID3D12Fence* m_pd3dFence = NULL;
	UINT64 m_nFenceValues[m_nSwapChainBuffers];
	HANDLE m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug* m_pd3dDebugController;
#endif
	CGameTimer m_GameTimer;	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�. 

	CScene* m_pScene;		//������ ���� �����ӿ�ũ���� ����� Ÿ�̸��̴�. 
	CPlayer* m_pPlayer = NULL;
	CCamera* m_pCamera = NULL;

	POINT m_ptOldCursorPos;

	_TCHAR m_pszFrameRate[70];
	
	ID3D12PipelineState* m_pd3dPipelineState;	// ������Ʈ ��� ������Ʈ���� ���� ����
};
