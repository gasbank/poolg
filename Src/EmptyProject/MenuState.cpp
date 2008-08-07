#include "EmptyProjectPCH.h"
#include "MenuState.h"
#include "WorldStateManager.h"

extern D3DXMATRIX g_orthoProjMat;
extern D3DXMATRIX g_fixedViewMat;

float pos = 90.0f;
int loc = 0;

void MenuState::select(int move)
{

	if ( loc == 0)
	{
		if ( move == 9 )
		{
			pos -= 35.0f;
			loc += 1;
		}
	}
	else if ( loc == 1 || loc == 2 || loc == 3 )
	{
		if (move == 8)
		{
			pos += 35.0f;
			loc -= 1;
		}
		if (move == 9)
		{
			pos -= 35.0f;
			loc += 1;
		}
	}
	else if ( loc == 4 )
	{
		if (move == 8)
		{
			pos += 35.0f;
			loc -= 1;
		}
	}
}



HRESULT MenuState::frameMove(double fTime, float fElapsedTime)
{
	m_menu.frameMove(fElapsedTime);
	m_selc.frameMove(fElapsedTime);
	
	m_stdb.frameMove(fElapsedTime);
	m_sadb.frameMove(fElapsedTime);
	
	m_selc.setPosition(m_selc.getPos()->x, pos, m_selc.getPos()->z);

	return S_OK;
}

HRESULT MenuState::frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime)
{
	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

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
	m_selc.draw();
	
	if ( loc == 0)
	{
		m_stdb.draw();
	}
	if ( loc == 1 )
	{
		m_sadb.draw();
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
			case '8':
				select( 8 );
				break;
			case '9':
				select( 9 );
				break;
			case VK_F5:
				WorldStateManager::getSingleton().setNextStateAsPrevState();
				break;
		}
	}

	return S_OK;
}


HRESULT MenuState::release()
{
	m_menu.release();
	m_selc.release();

	m_stdb.release();
	m_sadb.release();

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
	float menuBoxHeight = 300;

	float menuBoxPositionX = (float)scrWidth/2 - menuBoxWidth - 10;
	float menuBoxPositionY = (float)scrHeight/2 - menuBoxHeight - 8;
	m_menu.init(L"example_menu_window.png", m_pDev);
	m_menu.setPosition (menuBoxPositionX, menuBoxPositionY, 3.0f);
	m_menu.setSize(200, 300);

	float trianglePositionX = (float)scrWidth/2 - menuBoxWidth + 7;
	float triangleBoxPositionY = (float)scrHeight/2 - menuBoxWidth + pos;
	m_selc.init(L"triangle.png", m_pDev);
	m_selc.setPosition (trianglePositionX, triangleBoxPositionY, 2.9f);
	m_selc.setSizeToTexture();

	m_stdb.init(L"status_down_button.png", m_pDev);
	m_stdb.setPosition (198, 143, 2.7f);
	m_stdb.setSize(184, 44);

	m_sadb.init(L"save_down_button.png", m_pDev);
	m_sadb.setPosition (198, 91, 2.7f);
	m_sadb.setSize(184, 44);

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