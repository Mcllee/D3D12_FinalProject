#pragma once
#include "Scene.h"
#include "Timer.h"
#include "CPlayer.h"
// 게임 프로그램의 뼈대를 나타낸다.
// Direct3D Device를 생성하고 관리하며 화면 출력을 위한 여러 가지 처리 (게임 객체의 생성과 관리, 사용자 입력, 애니메이션 등)을 담당한다.
class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd); // 프레임워크를 초기화하는 함수이다(주 윈도우가 생성되면 호출된다).
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList(); // 스왑 체인, 디바이스, 서술자 힙, 명령큐/할당자/리스트를 생성하는 함수이다.

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView(); // 랜더 타겟 뷰와 깊이-스텐실 뷰를 생성하는 함수이다.

	void ChangeSwapChainState();	// 스왑체인

	void BuildObjects();
	void ReleaseObjects(); // 렌더링할 메쉬와 게임 객체를 생성하고 소멸하는 함수이다. 

	void ProcessInput();	// 프레임워크의 핵심(사용자 입력, 애니메이션, 렌더링)을 구성하는 함수이다.
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();		// CUP와 GPU를 동기화하는 함수이다.

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	// 윈도우의 메세지(키보드, 마우스 입력)를 처리하는 함수이다.

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	IDXGIFactory4* m_pdxgiFactory		= NULL;	// DXGI 팩토리 인터페이스에 대한 포인터이다.
	IDXGISwapChain3* m_pdxgiSwapChain	= NULL;	// 스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기위 필요하다.
	ID3D12Device* m_pd3dDevice			= NULL;	// Direct3D 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요하다.
	
	bool m_bMsaa4xEnable				= false;
	UINT m_nMsaa4xQualityLevels			= 0;	// MSAA 다중 샘플링을 활성화하고 다중 샘플링 레벨을 설정한다.

	static const UINT m_nSwapChainBuffers = 2;	// 스왑체인의 후면 버퍼의 개수이다.
	UINT m_nSwapChainBufferIndex;				// 현재 스왑 체인의 후면 버퍼 인덱스이다.

	ID3D12Resource* m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];	// 3D 모델용 스왑체인
	ID3D12Resource* m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// 기존 메쉬용 
	ID3D12DescriptorHeap* m_pd3dRtvDescriptorHeap = NULL;
	UINT m_nRtvDescriptorIncrementSize;									// 랜더 타겟 버퍼, 서술자 힙 인터페이스 포인터, 랜더 타겟 서술자 원소의 크기이다.

	ID3D12Resource* m_pd3dDepthStencilBuffer;
	ID3D12DescriptorHeap* m_pd3dDsvDescriptorHeap;
	UINT	m_nDsvDescriptorIncrementSize;								// 깊이-스텐실 버퍼, 서술자 힙 인터페이스 포인터, 깊이-스텐실 서술자 원소의 크기이다.

	ID3D12CommandAllocator* m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue* m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList* m_pd3dCommandList = NULL;				// 명령 큐, 명령 할당자, 명령 리스트 인터페이스 포인터이다.

	ID3D12Fence* m_pd3dFence = NULL;
	UINT64 m_nFenceValues[m_nSwapChainBuffers];
	HANDLE m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug* m_pd3dDebugController;
#endif
	CGameTimer m_GameTimer;	//다음은 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열이다. 

	CScene* m_pScene;		//다음은 게임 프레임워크에서 사용할 타이머이다. 
	CPlayer* m_pPlayer = NULL;
	CCamera* m_pCamera = NULL;

	POINT m_ptOldCursorPos;

	_TCHAR m_pszFrameRate[70];
	
	ID3D12PipelineState* m_pd3dPipelineState;	// 오브젝트 뷰어 프로젝트에는 없는 변수
};
