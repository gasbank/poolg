#include "EmptyProjectPCH.h"
#include "Dialog.h"
#include "SingletonCreators.h"
#include "ScriptManager.h"

extern D3DXMATRIX	g_orthoProjMat;
extern D3DXMATRIX	g_fixedViewMat;

LPD3DXFONT	Dialog::pFont;
RECT			Dialog::rc;

Dialog::Dialog(void)
{
	pFont = 0;
	m_pDev = GetG().m_dev;

	dial.init(L"dae-sa.png", m_pDev);
	dial.setPosition (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f), 2.8f);
	dial.setSizeToTexture();
}

Dialog::~Dialog(void)
{
}

HRESULT Dialog::init()
{
	HRESULT hr = S_OK;

	rc.top = GetG().m_scrHeight - 256 + 25;
	rc.left = 25;
	rc.right = 256 - 25;
	rc.bottom = GetG().m_scrHeight - 25;

	OK = false;

	V( D3DXCreateFont( GetG().m_dev, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &pFont) );

	return hr;
}

HRESULT Dialog::release()
{
	dial.release();

	SAFE_RELEASE( pFont );
	
	return S_OK;
}


int EpWriteDialog( const char* c )
{
	Dialog::pFont->DrawTextA(0, c, -1, &Dialog::rc, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );

	return 0;
} SCRIPT_CALLABLE_I_PC( EpWriteDialog )

START_SCRIPT_FACTORY(Dialog)
	CREATE_OBJ_COMMAND( EpWriteDialog )
END_SCRIPT_FACTORY(Dialog)

void Dialog::Toggle()
{
	if(	OK == true )
		OK = false;
	else
		OK = true;
}

HRESULT Dialog::frameMove(double fTime, float fElapsedTime)
{
	dial.frameMove(fElapsedTime);
	
	return S_OK;
}

HRESULT Dialog::frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime)
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
	
	if( OK )
	{
		dial.draw();

		GetScriptManager().execute("EpWorldState::printDialogText");

	}

	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	return S_OK;
}

HRESULT Dialog::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_KEYDOWN )
	{
		switch ( wParam )
		{
			case VK_SPACE:
				Toggle();
				break;
		}
	}

	return S_OK;
}