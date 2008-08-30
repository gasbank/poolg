#pragma once
#include "state.h"

//
// MainState
//
// 게임의 로고 타이틀과 함께
// Press any key to continue 정도의 메세지만 나오는 메인 메뉴. 배경으로는 실제 게임 세계를 그레이
// 스케일로 훑으면서 보여준다. 인트로 스테이트 뒤에 이어지는 스테이트이고, 게임이 끝났거나 플레이어가
// 죽었을 때에도 이 스테이트로 돌아온다. 아무 키나 눌렀을 때 다음에 이어질 세계는 스크립트로 지정할
// 수 있어야 한다. 이왕이면 메뉴의 배경으로 보여줄 스테이트도 스크립트로 지정하면 좋을 것이다.
//

class MainState :
	public State
{
public:
	MainState(void);
	virtual ~MainState(void);

	virtual HRESULT enter();
	virtual HRESULT leave();

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove(double fTime, float fElapsedTime);

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual void onLostDevice();

	virtual HRESULT release();
};
