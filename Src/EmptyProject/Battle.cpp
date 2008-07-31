#include "EmptyProjectPCH.h"
#include "Battle.h"

void Battle::init (LPDIRECT3DDEVICE9& d3dDev, int scrWidth, int scrHeight)
{
	HRESULT hr;
	m_pDev = d3dDev;

	float statusBoxWidth = 163;
	float statusBoxHeight = 124;

	float statusBoxPlayersPositionX = -(float)scrWidth/2 + 10;
	float statusBoxPlayersPositionY = -(float)scrHeight/2 + 10;
	m_StatusBoxPlayer.init(L"BattleUI\\StatusBox.png", d3dDev);
	m_StatusBoxPlayer.setPosition (statusBoxPlayersPositionX, statusBoxPlayersPositionY, 7);
	m_StatusBoxPlayer.setSize(statusBoxWidth, statusBoxHeight);

	float statusBoxEnemysPositionX = (float)scrWidth/2 - statusBoxWidth - 10;
	float statusBoxEnemysPositionY = (float)scrHeight/2 - statusBoxHeight - 10;
	m_StatusBoxEnemy.init(L"BattleUI\\StatusBox.png", d3dDev);
	m_StatusBoxEnemy.setPosition (statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f);
	m_StatusBoxEnemy.setSize(statusBoxWidth, statusBoxHeight);
	
	int skillBoxHeight = 200;
	int skillBoxWidth = skillBoxHeight * 593 / 933;
	int skillBoxPositionX = scrWidth/2 -skillBoxWidth - 3;
	int skillBoxPositionY =  -scrHeight/2 + 3;
	m_SkillBox.init(L"BattleUI\\SkillBox.png", d3dDev);
	m_SkillBox.setPosition ((float)skillBoxPositionX, (float)skillBoxPositionY, 6.9f);
	m_SkillBox.setSize((float)skillBoxWidth, (float)skillBoxHeight);

	int skillContentBoxHeight = 200;
	int skillContentBoxWidth = skillContentBoxHeight * 390 / 269;
	m_SkillContentBox.init(L"BattleUI\\SkillContentBox.png", d3dDev);
	m_SkillContentBox.setPosition ((float)skillBoxPositionX - skillContentBoxWidth, (float)skillBoxPositionY, 6.9f);
	m_SkillContentBox.setSize((float)skillContentBoxWidth, (float)skillContentBoxHeight);

	float dialogBoxWidth = (float)(int)(scrWidth - statusBoxWidth - 30);
	float dialogBoxHeight = 124;
	m_DialogBox.init(L"BattleUI\\DialogBox.png", d3dDev);
	m_DialogBox.setPosition ((float)-(scrWidth)/2 + 10, (float)(scrHeight)/2 - dialogBoxHeight - 10, 6.9f);
	//m_DialogBox.setPosition (10, 10, 6.9f);
	m_DialogBox.setSize(dialogBoxWidth, dialogBoxHeight);

	float statusBarWidth = statusBoxWidth * 0.67f;
	float statusBarHeight = statusBarWidth * 0.1f;
	m_hpBgPlayer.init(L"BattleUI\\HPbg.jpg", d3dDev);
	m_hpBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_mpBgPlayer.init(L"BattleUI\\MPbg.jpg", d3dDev);
	m_mpBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBgPlayer.init(L"BattleUI\\EXPbg.jpg", d3dDev);
	m_expBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 5);
	m_expBgPlayer.setSize(statusBarWidth, statusBarHeight);

	m_hpBgEnemy.init(L"BattleUI\\HPbg.jpg", d3dDev);
	m_hpBgEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgEnemy.setSize(statusBarWidth, statusBarHeight);
	m_mpBgEnemy.init(L"BattleUI\\MPbg.jpg", d3dDev);
	m_mpBgEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgEnemy.setSize(statusBarWidth, statusBarHeight);

	m_hpBarPlayer.init(L"BattleUI\\HPbar.jpg", d3dDev);
	m_hpBarPlayer.initRate();
	m_hpBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_hpBarPlayer.changeRate(-20);
	m_hpBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 4.5f);
	
	m_mpBarPlayer.init(L"BattleUI\\MPbar.jpg", d3dDev);
	m_mpBarPlayer.initRate();
	m_mpBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarPlayer.setSize(statusBarWidth, statusBarHeight);

	m_expBarPlayer.init(L"BattleUI\\EXPbar.jpg", d3dDev);
	m_expBarPlayer.initRate();
	m_expBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 4.5f);
	m_expBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBarPlayer.changeRate(-73.3f);

	m_hpBarEnemy.init(L"BattleUI\\HPbar.jpg", d3dDev);
	m_hpBarEnemy.initRate();
	m_hpBarEnemy.changeRate(-10);
	m_hpBarEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 4.5f);
	m_hpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	m_mpBarEnemy.init(L"BattleUI\\MPbar.jpg", d3dDev);
	m_mpBarEnemy.initRate();
	m_mpBarEnemy.changeRate(-3);
	m_mpBarEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	V( D3DXCreateFont(d3dDev, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblHYnamL) );
	V( D3DXCreateFont(d3dDev, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &m_lblREB) );





	//[À±¿í]
	m_menu.init(L"menu.png", d3dDev);
	m_menu.setPosition (8.5f, -2.0f, 4);
	m_menu.setSize(9.9f, 15.75f);

	m_selc.init(L"triangle.png", d3dDev);
	m_selc.setPosition (9.5f, 11.2f, 4);
	m_selc.setSize(0.75, 1.5);
}

void Battle::frameMove(float fElapsedTime)
{
	m_StatusBoxPlayer.frameMove(fElapsedTime);
	m_StatusBoxEnemy.frameMove(fElapsedTime);
	m_SkillBox.frameMove(fElapsedTime);
	m_SkillContentBox.frameMove(fElapsedTime);
	m_DialogBox.frameMove(fElapsedTime);
	m_hpBgPlayer.frameMove(fElapsedTime);
	m_mpBgPlayer.frameMove(fElapsedTime);
	m_expBgPlayer.frameMove(fElapsedTime);
	m_hpBgEnemy.frameMove(fElapsedTime);
	m_mpBgEnemy.frameMove(fElapsedTime);
	
	m_hpBarPlayer.frameMove(fElapsedTime);
	m_mpBarPlayer.frameMove(fElapsedTime);
	m_expBarPlayer.frameMove(fElapsedTime);

	m_hpBarEnemy.frameMove(fElapsedTime);
	m_mpBarEnemy.frameMove(fElapsedTime);

	//[À±¿í]
	m_menu.frameMove(fElapsedTime);
	m_selc.frameMove(fElapsedTime);
	
	
	
	//m_Player.frameMove(fElapsedTime);
	//m_Enemy.frameMove(fElapsedTime);
}

void Battle::draw(int scrWidth, int scrHeight)
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

	m_StatusBoxPlayer.draw();
	m_StatusBoxEnemy.draw();
	m_SkillBox.draw();
	m_SkillContentBox.draw();
	m_DialogBox.draw();
	m_hpBgPlayer.draw();
	m_mpBgPlayer.draw();
	m_expBgPlayer.draw();
	m_hpBgEnemy.draw();
	m_mpBgEnemy.draw();
	m_hpBarPlayer.draw();
	m_mpBarPlayer.draw();
	m_expBarPlayer.draw();
	m_hpBarEnemy.draw();
	m_mpBarEnemy.draw();

	drawFixedText(scrWidth, scrHeight);
	
//	D3DXMATRIX mRot, mScale, mTrans, mWorld;
//	D3DXMatrixRotationZ(&mRot, D3DXToRadian(45));
//	D3DXMatrixScaling(&mScale, 280.0f, 220.0f, 1.0f);
//	D3DXMatrixTranslation(&mTrans, 100.0f, -100.0f, 0);
//	mWorld = mScale * mRot * mTrans;
//	m_StatusBoxPlayer.setLocalXform(&mWorld);
//	
	
	//


	//[À±¿í]
	//m_menu.draw();
	//m_selc.draw();
	
}

void Battle::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	m_hpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_expBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarEnemy.handleMessages(hWnd, uMsg, wParam, lParam);
}

void Battle::release ()
{
	m_StatusBoxPlayer.release();
	m_StatusBoxEnemy.release();
	m_SkillBox.release();
	m_SkillContentBox.release();
	m_DialogBox.release();
	m_hpBgPlayer.release();
	m_mpBgPlayer.release();
	m_expBgPlayer.release();
	m_hpBgEnemy.release();
	m_mpBgEnemy.release();

	m_hpBarPlayer.release();
	m_mpBarPlayer.release();
	m_expBarPlayer.release();

	m_hpBarEnemy.release();
	m_mpBarEnemy.release();
	
	

	//[À±¿í]
	m_menu.release();
	m_selc.release();

	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB);
}

void Battle::drawFixedText(int scrWidth, int scrHeight)
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
	StringCchPrintf(textBuffer, 512, L"HYnamL ±Û¾¾Ã¼ÀÇ ¹¦¹Ì¸¦ ¸¾²¯ ´À²¸º¾½Ã´Ù.");
	m_lblHYnamL->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );


	float statusBoxPlayersPositionX = 10;
	float statusBoxPlayersPositionY = (float)(scrHeight - 10);
	rc.top = (LONG)(statusBoxPlayersPositionY - 102);
	rc.left = (LONG)(statusBoxPlayersPositionX + 5);
	StringCchPrintf(textBuffer, 512, L"HP");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

/*
	
	StringCchPrintf(debugBuffer, 512, L"«««á«éªÎêÈöÇ: (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"«««á«éªÎðÒªáªëíÞá¶: (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"«¿«¤«ëªÎêÈöÇ: (%.2f, %.2f, %.2f)", g_pic.getPos()->x, g_pic.getPos()->y, g_pic.getPos()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - »óÇÏÁÂ¿ì È­»ìÇ¥Å°¸¦ ÀÌ¿ëÇØ Ä«¸Þ¶ó¸¦ ÁÂ¿ì/°¡±îÀÌ¸Ö¸® ÀÌµ¿ °¡´É");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"   (Ä«¸Þ¶ó´Â Ç×»ó +Z ÃàÀ» ¹Ù¶óº¸°Ô µÊ)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - I/K/J/LÅ°¸¦ ÀÌ¿ëÇØ ¸Ê ÀÌ¹ÌÁö¸¦ »óÇÏÁÂ¿ì·Î ÀÌµ¿ °¡´É");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - ½ºÆäÀÌ½ºÅ°¸¦ ÀÌ¿ëÇØ Æ¯¼ö±â´É ÀÛµ¿ (´©¸£°í ÀÖÀ¸¸é ÇÊ»ì°ø°Ý)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

*/
}
