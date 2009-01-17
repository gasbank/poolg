#pragma once
#include "character.h"


/**
@brief 적군 캐릭터를 다루는 클래스

적군 캐릭터를 다루는 클래스입니다.

주 기능은 맵 상을 랜덤하게 돌아다니는 것, 말을 걸었을 때 나오는 동작등입니다.
또한, 적군을 무찔렀을 때 받게 되는 보상에 대해서도 설정할 수 있습니다.
*/
class Enemy :
	public Character
{
public:
	// Ctor and Dtor and create
	static Unit*				createEnemy( LPD3DXMESH mesh, int tileX, int tileY, float posZ );
	virtual						~Enemy(void);

	// VIrtual Methods
	virtual HRESULT				frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime );
	virtual bool				frameMove( double dTime, float fElapsedTime );
	virtual LRESULT				handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void						setRandomWalkable( bool randomWalkable )	{ m_bRandomWalkable = randomWalkable; }
	void						setTalkable( bool talkable )				{ m_bTalkable = talkable; }
	bool						isTalkable() const							{ return m_bTalkable; }
	void						setFightRange( int i )						{ m_iFightRange = i; }
	int							getFightRange() const						{ return m_iFightRange; }
	int							getExpReward() const						{ return m_expReward; }
	void						setExpReward( int exp )						{ m_expReward = exp; }

private:
								Enemy();
	void						walkRandomly();

	bool						m_bRandomWalkable;
	bool						m_bTalkable;
	int							m_iFightRange;
	int							m_expReward;
};


SCRIPT_FACTORY( Enemy )
