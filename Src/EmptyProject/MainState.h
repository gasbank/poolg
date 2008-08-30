#pragma once
#include "state.h"

//
// MainState
//
// ������ �ΰ� Ÿ��Ʋ�� �Բ�
// Press any key to continue ������ �޼����� ������ ���� �޴�. ������δ� ���� ���� ���踦 �׷���
// �����Ϸ� �����鼭 �����ش�. ��Ʈ�� ������Ʈ �ڿ� �̾����� ������Ʈ�̰�, ������ �����ų� �÷��̾
// �׾��� ������ �� ������Ʈ�� ���ƿ´�. �ƹ� Ű�� ������ �� ������ �̾��� ����� ��ũ��Ʈ�� ������
// �� �־�� �Ѵ�. �̿��̸� �޴��� ������� ������ ������Ʈ�� ��ũ��Ʈ�� �����ϸ� ���� ���̴�.
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
