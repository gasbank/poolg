#pragma once
#include "State.h"

class World;
class ArnSceneGraph;
class Sprite;

/**
@brief 게임을 플레이하고 있는 중의 State
*/
class PlayState : public State
{
public:
							PlayState(void);
							~PlayState(void);
	virtual HRESULT			enter( double dStartTime );
	virtual HRESULT			leave();
	virtual HRESULT			frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT			frameMove(double fTime, float fElapsedTime);
	virtual HRESULT			handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual HRESULT			onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual HRESULT			onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual void			onLostDevice();
	virtual HRESULT			release();
	ArnSceneGraph*			getCharacterSceneGraph() const;
	void					updateHeroItemListThumbnails();

private:
	void					loadArnModels();
	void					unloadArnModels();
	ArnSceneGraph*			m_CharactersSg;
	Sprite*					m_sprite;
};
