#include "EmptyProjectPCH.h"
#include "MenuState.h"
#include "WorldStateManager.h"

int loc = 4;
bool op_st = false;
bool op_sa = false;
bool op_lo = false;

void MenuState::select(int move)
{
	if ( loc == 0)
	{
		if ( move == 9 )
		{
			loc += 1;
		}
	}
	else if ( loc == 1 || loc == 2 || loc == 3 )
	{
		if (move == 8)
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
	}
	

	if ( loc == 0 )
	{
		if ( move == 0 )
		{
			if ( op_st == false )
			{
				op_st = true;
			}
			else op_st = false;
		}
	}

	if ( loc == 1 )
	{
		if ( move == 0 )
		{
			if ( op_sa == false )
			{
				op_sa = true;
			}
			else op_sa = false;
		}
	}

	if ( loc == 2 )
	{
		if ( move == 0 )
		{
			if ( op_lo == false )
			{
				op_lo = true;
			}
			else op_lo = false;
		}
	}

	if ( loc ==4 )
		if ( move == 0 )
			SendMessage( DXUTGetHWND(), WM_CLOSE, 0, 0 );
}



HRESULT MenuState::frameMove(double fTime, float fElapsedTime)
{
	m_menu.frameMove(fElapsedTime);

	//m_sanub.frameMove(fElapsedTime);

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
	
	//m_selc.setPos(m_selc.getPos()->x, pos, m_selc.getPos()->z);

	return S_OK;
}

HRESULT MenuState::frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime)
{
	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

	D3DMATERIAL9 material;
	D3DCOLORVALUE cv, cv2;
	cv.a = 0.3f; cv.r = 0.1f; cv.g = 0.1f; cv.b = 0.1f;
	cv2.a = 0.3f; cv2.r = 1.0f; cv2.g = 0.1f; cv2.b = 0.3f;
	material.Ambient = cv;
	material.Diffuse = cv2;
	material.Emissive = cv;
	material.Power = 1.0f;
	material.Specular = cv2;
	m_pDev->SetMaterial(&material);

	//drawFixedText(scrWidth, scrHeight);

	m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_menu.draw();
	
	m_stub.draw();
	//if ( m_nextState == m_states[1] )
		m_sanub.draw();
	//else m_saub.draw();
	m_loub.draw();
	m_seub.draw();
	m_exub.draw();

	switch ( loc )
	{
	case 0 :
		m_stdb.draw();
		break;
	case 1 :
		//if ( m_nextState != m_states[1] )
			m_sadb.draw();
		break;
	case 2 :
		m_lodb.draw();
		break;
	case 3 :
		m_sedb.draw();
		break;
	case 4 :
		m_exdb.draw();
		break;
	}

	if ( op_st == true )
	{
		m_stwin.draw();
	}
	if ( op_sa == true )
	{
		m_sawin.draw();
	}
	if ( op_lo == true )
	{
		m_lowin.draw();
	}
	
	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
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
				break;
			case VK_DOWN:
				if ( !op_st && !op_sa && !op_lo )
					select( 9 );
				break;
			case VK_RETURN:
				select( 0 );
				break;
			case VK_F5:
				GetWorldStateManager().setNextStateAsPrevState();
				break;
		}
	}

	return S_OK;
}


HRESULT MenuState::release()
{
	m_menu.release();

	//m_sanub.release();

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

	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB);

	return S_OK;
}

MenuState::MenuState()
{
	HRESULT hr;
	m_pDev = GetG().m_dev;
	float scrWidth = (float)GetG().m_scrWidth;
	float scrHeight = (float)GetG().m_scrHeight;

	float menuBoxWidth = 200;
	float menuBoxHeight = 264;

	float menuButtonWidth = 184;
	float menuButtonHeight = 44;

	float menuBoxPositionX = (float)scrWidth/2 - menuBoxWidth - 12;
	float menuBoxPositionY = (float)scrHeight/2 - menuBoxHeight - 20;
	m_menu.init(L"Images/menu_window.png", m_pDev);
	m_menu.setPos (menuBoxPositionX, menuBoxPositionY, 3.0f);
	m_menu.setSize( menuBoxWidth, menuBoxHeight );

	//float trianglePositionX = (float)scrWidth/2 - menuBoxWidth + 7;
	//float triangleBoxPositionY = (float)scrHeight/2 - menuBoxWidth + pos;

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
	m_exdb.setSize( menuButtonWidth, menuButtonHeight);

	m_stwin.init(L"Images/status_window.png", m_pDev);
	m_stwin.setPos ( 0 - scrWidth / 2, 0 - scrHeight / 2, 2.6f);
	m_stwin.setSize(scrWidth, scrHeight);

	m_sawin.init(L"Images/save_window.png", m_pDev);
	m_sawin.setPos ( 0 - scrWidth / 2, 0 - scrHeight / 2, 2.6f);
	m_sawin.setSize(scrWidth, scrHeight);

	m_lowin.init(L"Images/load_window.png", m_pDev);
	m_lowin.setPos ( 0 - scrWidth / 2, 0 - scrHeight / 2, 2.6f);
	m_lowin.setSize(scrWidth, scrHeight);


	V( D3DXCreateFont(m_pDev, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblHYnamL) );
	V( D3DXCreateFont(m_pDev, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &m_lblREB) );
}

MenuState::~MenuState()
{

}

HRESULT MenuState::enter()
{

	return S_OK;
}

HRESULT MenuState::leave()
{

	return S_OK;
}
/*
void MenuState::drawFixedText(int scrWidth, int scrHeight)
{
	WCHAR textBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = scrWidth;
	rc.bottom = scrHeight;
	StringCchPrintf(textBuffer, 512, L"TextTest!!!!");
	m_lblHYnamL->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(textBuffer, 512, L"HYnamL ±Û¾¾Ã¼ÀÇ ¹¦¹Ì¸¦ ¸¾²¯ ´À²¸º¾½Ã´Ù. W/A/S/D·Î ÁÖÀüÀÚ ÀÌµ¿ °¡´É!!!");
	m_lblHYnamL->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );


	float statusBoxPlayersPositionX = 10;
	float statusBoxPlayersPositionY = (float)(scrHeight - 10);
	rc.top = (LONG)(statusBoxPlayersPositionY - 102);
	rc.left = (LONG)(statusBoxPlayersPositionX + 5);
	StringCchPrintf(textBuffer, 512, L"HP");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

*/