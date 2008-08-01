#include "EmptyProjectPCH.h"
#include "Menu.h"

void Menu::init (LPDIRECT3DDEVICE9& d3dDev, int scrWidth, int scrHeight)
{
	HRESULT hr;
	m_pDev = d3dDev;

	float menuBoxWidth = 158;
	float menuBoxHeight = 127;
	float select = 25;

	float menuBoxPositionX = (float)scrWidth/2 - menuBoxWidth - 10;
	float menuBoxPositionY = (float)scrHeight/2 - menuBoxWidth - 70;
	m_menu.init(L"menu.png", d3dDev);
	m_menu.setPosition (menuBoxPositionX, menuBoxPositionY, 3.0f);
	m_menu.setSize(menuBoxWidth, menuBoxHeight);

	float trianglePositionX = (float)scrWidth/2 - menuBoxWidth + 5;
	float triangleBoxPositionY = (float)scrHeight/2 - menuBoxWidth + select;
	m_selc.init(L"triangle.png", d3dDev);
	m_selc.setPosition (trianglePositionX, triangleBoxPositionY, 2.9f);
	m_selc.setSize(10, 18);

	V( D3DXCreateFont(d3dDev, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblHYnamL) );
	V( D3DXCreateFont(d3dDev, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &m_lblREB) );
}

void Menu::frameMove(float fElapsedTime)
{
	m_menu.frameMove(fElapsedTime);
	m_selc.frameMove(fElapsedTime);
	
}

void Menu::draw(int scrWidth, int scrHeight)
{
	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	m_menu.draw();
	m_selc.draw();
	
}
/*
void menu::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	m_hpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_expBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarEnemy.handleMessages(hWnd, uMsg, wParam, lParam);
}
*/

void Menu::release ()
{
	m_menu.release();
	m_selc.release();

	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB);
}
/*
void Menu::drawFixedText(int scrWidth, int scrHeight)
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