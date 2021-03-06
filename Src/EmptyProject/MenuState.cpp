#include "EmptyProjectPCH.h"
#include "MenuState.h"
#include "WorldStateManager.h"
#include "Character.h"
#include "World.h"
#include "TopStateManager.h"
#include "WorldStateManager.h"
#include "Hero.h"
#include "Sprite.h"
#include "SpriteManager.h"

int loc;
int slb;

float curHp = 0;
float maxHp = 0;
float curCs = 0;
float maxCs = 0;
float curExp = 0;
float maxExp = 0;
bool op_st = false;
bool op_sa = false;
bool op_lo = false;
const UINT menuButtonWidth = 184;
const UINT menuButtonHeight = 44;

extern LPD3DXFONT	g_fontBattle;

void MenuState::select(int move)
{
	if ( move == 0 )
	{
		if ( m_curSelMenu == 0 )
		{
			if ( op_st == false )
			{
				op_st = true;
			}
			else op_st = false;
		}
		else if ( m_curSelMenu == 1 )
		{
			if ( op_sa == false )
			{
				op_sa = true;
			}
			else op_sa = false;
		}
		else if ( m_curSelMenu == 2 )
		{
			if ( op_lo == false )
			{
				op_lo = true;
			}
			else op_lo = false;
		}
		else if ( m_curSelMenu == 4 )
			SendMessage( DXUTGetHWND(), WM_CLOSE, 0, 0 );
	}


	if ( move == 8 || move == 9 )
	{
		m_menuDr[ m_curSelMenu ]->srcRect.top		+= menuButtonHeight * 5;
		m_menuDr[ m_curSelMenu ]->srcRect.bottom	+= menuButtonHeight * 5;

		
		if(GetWorldStateManager().prevStateEnum() == GAME_WORLD_STATE_BATTLE)
		{
			if ( move == 8 ) m_curSelMenu += 2;
			if ( move == 9 ) m_curSelMenu -= 2;
		}
		else if ( move == 8 ) --m_curSelMenu;
		else if ( move == 9 ) ++m_curSelMenu;

		if ( m_curSelMenu < 0 )
			m_curSelMenu += 5;
		else if ( m_curSelMenu > 4 )
			m_curSelMenu = 0;


		m_menuDr[ m_curSelMenu ]->srcRect.top		-= menuButtonHeight * 5;
		m_menuDr[ m_curSelMenu ]->srcRect.bottom	-= menuButtonHeight * 5;

	}



	/*if ( loc == 0)
	{
		if(GetWorldStateManager().prevStateEnum() == GAME_WORLD_STATE_BATTLE)
		{
			if ( move == 9 )
			{
				loc += 2;
			}
		}
		else if ( move == 9 )
		{
			loc += 1;
		}
		if ( move == 8 )
		{
			loc += 4;
		}
	}
	else if ( loc == 1 || loc == 2 || loc == 3 )
	{
		if(GetWorldStateManager().prevStateEnum() == GAME_WORLD_STATE_BATTLE && loc == 2)
		{
			if( move == 8 )
			{
				loc -= 2;
			}
		}
		else if ( move == 8 )
		{
			loc -= 1;
		}
		if (move == 9)
		{
			loc += 1;
		}
	}
	else if ( loc == 4 )
	{				
		if ( move == 8 )
		{
			loc -= 1;
		}
		if ( move == 9 )
		{
			loc -= 4;
		}
	}

	if ( loc == 0 )
	{
		if ( move == 0 )
		{
			
		}
	}

	if ( loc == 1 )
	{
		if ( move == 0 )
		{
			
		}
	}
	if ( loc == 2 )
	{
		if ( move == 0 )
		{
			
		}
	}

	if ( loc ==4 )
	{
		if ( move == 0 )
			SendMessage( DXUTGetHWND(), WM_CLOSE, 0, 0 );
	}*/
}

void MenuState::saveloadselect(int select)
{
	
	if ( slb == 0 )
	{
		if ( select == 8 )
		{
			slb += 3;
		}
		else if ( select == 9 )
		{
			slb += 1;
		}
	}
	else if ( slb == 1 || slb ==2 )
	{
		if ( select == 8 )
		{
			slb -= 1;
		}
		else if ( select == 9 )
		{
			slb += 1;
		}
	}
	else if ( slb == 3 )
	{
		if ( select == 8 )
		{
			slb -= 1;
		}
		else if ( select == 9 )
		{
			slb -= 3;
		}
	}
}




HRESULT MenuState::frameMove(double fTime, float fElapsedTime)
{
	/*
	m_menu.frameMove(fElapsedTime);

	m_sanub.frameMove(fElapsedTime);

	m_stub.frameMove(fElapsedTime);
	m_saub.frameMove(fElapsedTime);
	m_loub.frameMove(fElapsedTime);
	m_seub.frameMove(fElapsedTime);
	m_exub.frameMove(fElapsedTime);
	
	m_stdb.frameMove(fElapsedTime);
	m_sadb.frameMove(fElapsedTime);
	m_lodb.frameMove(fElapsedTime);
	m_sedb.frameMove(fElapsedTime);
	m_exdb.frameMove(fElapsedTime);

	m_stwin.frameMove(fElapsedTime);
	m_sawin.frameMove(fElapsedTime);
	m_lowin.frameMove(fElapsedTime);

	m_hpbar.frameMove(fElapsedTime);
	m_mpbar.frameMove(fElapsedTime);
	m_expbar.frameMove(fElapsedTime);

	m_hpbg.frameMove(fElapsedTime);
	m_mpbg.frameMove(fElapsedTime);
	m_expbg.frameMove(fElapsedTime);

	m_slb0.frameMove(fElapsedTime);
	m_slb1.frameMove(fElapsedTime);
	m_slb2.frameMove(fElapsedTime);
	m_slb3.frameMove(fElapsedTime);
	*/
	
	//m_selc.setPos(m_selc.getPos()->x, pos, m_selc.getPos()->z);

	return S_OK;
}

HRESULT MenuState::frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime)
{
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, (const D3DXMATRIX*)GetG().g_fixedViewMat.m);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, (const D3DXMATRIX*)GetG().g_orthoProjMat.m);

	GetSpriteManager().frameRenderSpecificSprite( "MenuUI" );

	D3DMATERIAL9 material;
	D3DCOLORVALUE cv, cv2;
	cv.a = 0.3f; cv.r = 0.1f; cv.g = 0.1f; cv.b = 0.1f;
	cv2.a = 0.3f; cv2.r = 1.0f; cv2.g = 0.1f; cv2.b = 0.3f;
	material.Ambient = cv;
	material.Diffuse = cv2;
	material.Emissive = cv;
	material.Power = 1.0f;
	material.Specular = cv2;
	pd3dDevice->SetMaterial(&material);

	pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	if ( op_st == true )
	{
		/*
		m_stwin.draw();

		m_hpbg.draw();
		m_mpbg.draw();
		m_expbg.draw();

		m_hpbar.draw();
		m_mpbar.draw();
		*/

		RECT rc;
		RECT rc2;
		RECT rc3;
		rc.top		= 110;
		rc.left		= 25;
		rc.right	= 800;
		rc.bottom	= 600;

		rc2.top		= (LONG)600-364;;
		rc2.left	= (LONG)800-204;
		rc2.right	= (LONG)800-69;
		rc2.bottom	= 600;

		rc3.top		= 62;
		rc3.left	= 446;
		rc3.right	= 500;
		rc3.bottom	= 600;

		DWORD dtProp = DT_NOCLIP | DT_CENTER;
		D3DXCOLOR textColor = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

		World* ws = getCurWorld();

		WCHAR textBuffer[512];
		rc.top = (LONG)600-364;
		rc.left = (LONG)800-343;
		rc.right = (LONG)800-205;
		StringCchPrintf(textBuffer, 512, L"Health");
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc, dtProp, textColor);
		rc.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"Will");
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc, dtProp, textColor );
		rc.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"Coding");
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc, dtProp, textColor );
		rc.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"Defence");
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc, dtProp, textColor );
		rc.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"Sense");
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc, dtProp, textColor );
		rc.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"Immunity");
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc, dtProp, textColor );
		
		StringCchPrintf(textBuffer, 512, L"%d", ws->getHeroUnit()->getStat().health );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc2, dtProp, textColor );
		rc2.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"%d", ws->getHeroUnit()->getStat().will );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc2, dtProp, textColor );
		rc2.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"%d", ws->getHeroUnit()->getStat().coding );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc2, dtProp, textColor );
		rc2.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"%d", ws->getHeroUnit()->getStat().def );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc2, dtProp, textColor );
		rc2.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"%d", ws->getHeroUnit()->getStat().sense );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc2, dtProp, textColor );
		rc2.top += (LONG)47;
		StringCchPrintf(textBuffer, 512, L"%d", ws->getHeroUnit()->getStat().immunity );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc2, dtProp, textColor );

		StringCchPrintf(textBuffer, 512, L"PoolG" );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc3, DT_NOCLIP | DT_LEFT, textColor );
		rc3.top += (LONG)25;
		StringCchPrintf(textBuffer, 512, L"He is a mouse who live in the ceiling of" );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc3, DT_NOCLIP | DT_LEFT, textColor );
		rc3.top += (LONG)19;
		StringCchPrintf(textBuffer, 512, L"basement floor in Engineering Hall I." );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc3, DT_NOCLIP | DT_LEFT, textColor );
		rc3.top += (LONG)19;
		StringCchPrintf(textBuffer, 512, L"One day, People did 'Big Cleaning'." );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc3, DT_NOCLIP | DT_LEFT, textColor );
		rc3.top += (LONG)19;
		StringCchPrintf(textBuffer, 512, L"So, Living place of mice is decreased." );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc3, DT_NOCLIP | DT_LEFT, textColor );
		rc3.top += (LONG)19;
		StringCchPrintf(textBuffer, 512, L"And then he goes to repulse the enemy." );
		g_fontBattle->DrawTextW(0, textBuffer, -1, &rc3, DT_NOCLIP | DT_LEFT, textColor );

	}


	/*
	if ( op_sa == true )
	{
		m_sawin.draw();
		switch ( slb )
		{
		case 0:
			m_slb0.draw();
			break;
		case 1:
			m_slb1.draw();
			break;
		case 2:
			m_slb2.draw();
			break;
		case 3:
			m_slb3.draw();
			break;
		}
	}
	if ( op_lo == true )
	{
		m_lowin.draw();
		switch ( slb )
		{
		case 0:
			m_slb0.draw();
			break;
		case 1:
			m_slb1.draw();
			break;
		case 2:
			m_slb2.draw();
			break;
		case 3:
			m_slb3.draw();
			break;
		}
	}
	*/
	
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	return S_OK;
	
}

HRESULT MenuState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_KEYDOWN )
	{
		switch ( wParam )
		{

			case VK_UP:
				if ( !op_st && !op_sa && !op_lo )
					select( 8 );
				else if ( op_sa || op_lo )
					saveloadselect( 8 );
				break;
			case VK_DOWN:
				if ( !op_st && !op_sa && !op_lo )
					select( 9 );
				else if ( op_sa || op_lo )
					saveloadselect( 9 );
				break;
			case VK_RETURN:
				select( 0 );
				break;
			case VK_ESCAPE:
				if ( op_st || op_sa || op_lo )
					select( 0 );
				GetWorldStateManager().setNextStateAsPrevState();
				break;
		}
	}

	return S_OK;
}


HRESULT MenuState::release()
{
	/*
	m_menu.release();

	m_sanub.release();

	m_stub.release();
	m_saub.release();
	m_loub.release();
	m_seub.release();
	m_exub.release();

	m_stdb.release();
	m_sadb.release();
	m_lodb.release();
	m_sedb.release();
	m_exdb.release();

	m_stwin.release();
	m_sawin.release();
	m_lowin.release();

	m_hpbar.release();
	m_mpbar.release();
	m_expbar.release();

	m_hpbg.release();
	m_mpbg.release();
	m_expbg.release();

	m_slb0.release();
	m_slb1.release();
	m_slb2.release();
	m_slb3.release();
	*/


	return S_OK;
}

MenuState::MenuState()
{
	const UINT menuBoxWidth = 200;
	const UINT menuBoxHeight = 264;
	const int scrWidth_i = GetG().m_scrWidth;
	const int scrHeight_i = GetG().m_scrHeight;
	float scrWidth_f = (float)GetG().m_scrWidth;
	float scrHeight_f = (float)GetG().m_scrHeight;

	m_sprite = GetSpriteManager().registerSprite( "MenuUI", "Images/menu_images.png" );
	m_sprite->setCustomRendered( true );
	m_sprite->registerRect( "MenuBox", menuButtonWidth, 0, menuButtonWidth+menuBoxWidth, menuBoxHeight);
	m_sprite->registerRect( "Status_Unpushed", 0, 0, 0+menuButtonWidth, 0+menuButtonHeight*1 );
	m_sprite->registerRect( "Save_Unpushed", 0, 0+menuButtonHeight*6, 0+menuButtonWidth, 0+menuButtonHeight*7 );
	m_sprite->registerRect( "Load_Unpushed", 0, 0+menuButtonHeight*7, 0+menuButtonWidth, 0+menuButtonHeight*8 );
	m_sprite->registerRect( "Setting_Unpushed", 0, 0+menuButtonHeight*8, 0+menuButtonWidth, 0+menuButtonHeight*9 );
	m_sprite->registerRect( "Exit_Unpushed", 0, 0+menuButtonHeight*9, 0+menuButtonWidth, 0+menuButtonHeight*10 );

	m_sprite->drawRequest( "MenuBox", 0, scrWidth_i - menuBoxWidth - 12, 5, 0, 0xffffffff );
	m_menuDr[0] = m_sprite->drawRequest( "Status_Unpushed", 0, scrWidth_i - menuButtonWidth - 20, menuButtonHeight - 30, 0, 0xffffffff );
	m_menuDr[1] = m_sprite->drawRequest( "Save_Unpushed", 0, scrWidth_i - menuButtonWidth - 20, menuButtonHeight + 20, 0, 0xffffffff );
	m_menuDr[2] = m_sprite->drawRequest( "Load_Unpushed", 0, scrWidth_i - menuButtonWidth - 20, menuButtonHeight + 70, 0, 0xffffffff );
	m_menuDr[3] = m_sprite->drawRequest( "Setting_Unpushed", 0, scrWidth_i - menuButtonWidth - 20, menuButtonHeight + 120, 0, 0xffffffff );
	m_menuDr[4] = m_sprite->drawRequest( "Exit_Unpushed", 0, scrWidth_i - menuButtonWidth - 20, menuButtonHeight + 170, 0, 0xffffffff );

	m_curSelMenu = 0;

	/*
	float menuBoxPositionX = (float)scrWidth/2 - menuBoxWidth - 12;
	float menuBoxPositionY = (float)scrHeight/2 - menuBoxHeight - 20;
	m_menu.init(L"Images/menu_window.png", m_pDev);
	m_menu.setPos (menuBoxPositionX, menuBoxPositionY, 3.0f);
	m_menu.setSize( menuBoxWidth, menuBoxHeight );

	m_stub.init(L"Images/status_up_button.png", m_pDev);
	m_stub.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 30, 2.8f);
	m_stub.setSize( menuButtonWidth, menuButtonHeight);

	m_saub.init(L"Images/save_up_button.png", m_pDev);
	m_saub.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 80, 2.8f);
	m_saub.setSize( menuButtonWidth, menuButtonHeight);

	m_sanub.init(L"Images/save_unusable_button.png", m_pDev);
	m_sanub.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 80, 2.8f);
	m_sanub.setSize( menuButtonWidth, menuButtonHeight);

	m_loub.init(L"Images/load_up_button.png", m_pDev);
	m_loub.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 130, 2.8f);
	m_loub.setSize( menuButtonWidth, menuButtonHeight);

	m_seub.init(L"Images/setting_up_button.png", m_pDev);
	m_seub.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 180, 2.8f);
	m_seub.setSize( menuButtonWidth, menuButtonHeight);

	m_exub.init(L"Images/exit_up_button.png", m_pDev);
	m_exub.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 230, 2.8f);
	m_exub.setSize( menuButtonWidth, menuButtonHeight);


	m_stdb.init(L"Images/status_down_button.png", m_pDev);
	m_stdb.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 30, 2.7f);
	m_stdb.setSize( menuButtonWidth, menuButtonHeight);

	m_sadb.init(L"Images/save_down_button.png", m_pDev);
	m_sadb.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 80, 2.7f);
	m_sadb.setSize( menuButtonWidth, menuButtonHeight);

	m_lodb.init(L"Images/load_down_button.png", m_pDev);
	m_lodb.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 130, 2.7f);
	m_lodb.setSize( menuButtonWidth, menuButtonHeight);

	m_sedb.init(L"Images/setting_down_button.png", m_pDev);
	m_sedb.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 180, 2.7f);
	m_sedb.setSize( menuButtonWidth, menuButtonHeight);

	m_exdb.init(L"Images/exit_down_button.png", m_pDev);
	m_exdb.setPos ( (float)scrWidth/2 - menuButtonWidth - 20, (float)scrHeight/2 - menuButtonHeight - 230, 2.7f);
	m_exdb.setSize( menuButtonWidth, menuButtonHeight);*/


	//////////////////////////////////////////////////////////////////////////
	// [START] It is commented since Picture class is deprecated!

	/*
	m_stwin.init(L"Images/status_window.png", m_pDev);
	m_stwin.setPos ( 0 - scrWidth_f / 2, 0 - scrHeight_f / 2, 2.6f);
	m_stwin.setSize(scrWidth_f, scrHeight_f);

	m_sawin.init(L"Images/save_window.png", m_pDev);
	m_sawin.setPos ( 0 - scrWidth_f / 2, 0 - scrHeight_f / 2, 2.6f);
	m_sawin.setSize(scrWidth_f, scrHeight_f);

	m_lowin.init(L"Images/load_window.png", m_pDev);
	m_lowin.setPos ( 0 - scrWidth_f / 2, 0 - scrHeight_f / 2, 2.6f);
	m_lowin.setSize( scrWidth_f, scrHeight_f);

	m_hpbar.init(L"Images/BattleUI/HPbar.jpg", m_pDev);
	m_hpbar.setPos ( (0 - scrWidth_f / 2) + 163, (0 - scrHeight_f / 2) + 223, 2.4f);

	m_hpbg.init(L"Images/BattleUI/HPbg.jpg", m_pDev);
	m_hpbg.setPos ( (0 - scrWidth_f / 2) + 163, (0 - scrHeight_f / 2) + 223, 2.5f);
	m_hpbg.setSize(220, 22);

	m_mpbar.init(L"Images/BattleUI/DKbar.jpg", m_pDev);
	m_mpbar.setPos ( (0 - scrWidth_f / 2) + 163, (0 - scrHeight_f / 2) + 171, 2.4f);

	m_mpbg.init(L"Images/BattleUI/MPbg.jpg", m_pDev);
	m_mpbg.setPos ( (0 - scrWidth_f / 2) + 163, (0 - scrHeight_f / 2) + 171, 2.5f);
	m_mpbg.setSize(220, 22);
	
	m_expbar.init(L"Images/BattleUI/EXPbar.jpg", m_pDev);
	m_expbar.setPos ( (0 - scrWidth_f / 2) + 163, (0 - scrHeight_f / 2) + 119, 2.4f);

	m_expbg.init(L"Images/BattleUI/EXPbg.jpg", m_pDev);
	m_expbg.setPos ( (0 - scrWidth_f / 2) + 163, (0 - scrHeight_f / 2) + 119, 2.5f);
	m_expbg.setSize(220, 22);

	m_slb0.init(L"Images/save_load_selected.png", m_pDev);
	m_slb0.setPos ( -342, 86, 2.5f);
	m_slb0.setSize(684, 112);

	m_slb1.init(L"Images/save_load_selected.png", m_pDev);
	m_slb1.setPos ( -342, 86 - 122, 2.5f);
	m_slb1.setSize(684, 112);

	m_slb2.init(L"Images/save_load_selected.png", m_pDev);
	m_slb2.setPos ( -342, 86 - 244, 2.5f);
	m_slb2.setSize(684, 112);

	m_slb3.init(L"Images/save_load_selected.png", m_pDev);
	m_slb3.setPos ( -342, 86 - 366, 2.5f);
	m_slb3.setSize(684, 112);
	*/
	
	// [END] It is commented since Picture class is deprecated!
	//////////////////////////////////////////////////////////////////////////

}

MenuState::~MenuState()
{
}

HRESULT MenuState::enter( double dStartTime )
{
	TopStateManager& ts = GetTopStateManager();
	World* ws = getCurWorld();
	ws->getHero()->clearKey();

	if ( ws->getHero() != 0 )
	{
		maxHp = (float)( (Character*) ws->getHero() )->getMaxHp();
		curHp = (float)( (Character*) ws->getHero() )->getCurHp();

		curCs = (float)( (Character*) ws->getHero() )->getCurCs();
		maxCs = (float)( (Character*) ws->getHero() )->getMaxCs();

		curExp = (float) ( (Hero*) ws->getHero() )->getCurExp();
		maxExp = (float) ( (Hero*) ws->getHero() )->getMaxExp();

		/*
		m_hpbar.setSize(220 * (curHp / maxHp), 22);
		m_mpbar.setSize(220 * (curCs / maxCs), 22);
		m_expbar.setSize(220 * (curExp / maxExp), 22);
		*/
	}

	loc = 0;
	slb = 0;

	return State::enter( dStartTime );
}

HRESULT MenuState::leave()
{
	return State::leave();
}

HRESULT MenuState::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}

void MenuState::onLostDevice()
{	
}

HRESULT MenuState::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr = S_OK;
	return hr;
}
