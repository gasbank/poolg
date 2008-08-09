#include "EmptyProjectPCH.h"
#include "Dialog.h"
#include "SingletonCreators.h"
#include "ScriptManager.h"

LPD3DXFONT	Dialog::pFont;
RECT		Dialog::pRc;
LPD3DXFONT	Dialog::qFont;
RECT		Dialog::qRc;

Dialog::Dialog(void)
{
	pFont = 0;
	qFont = 0;
	m_pDev = GetG().m_dev;

	dialog.init(L"dae-sa.png", m_pDev);
	dialog.setPosition (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f), 2.8f);
	dialog.setSize(200, 200);
	name.init(L"dae-sa.png", m_pDev);
	name.setPosition (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f) + 200, 2.8f);
	name.setSize(21 + 40, 13 + 40);
}

Dialog::~Dialog(void)
{
}

HRESULT Dialog::init()
{
	HRESULT hr = S_OK;

	pRc.top = GetG().m_scrHeight - 200 + 20;
	pRc.left = 20;
	pRc.right = 200 - 20;
	pRc.bottom = GetG().m_scrHeight - 20;
	qRc.top = (GetG().m_scrHeight - 200) - 61 + 20;
	qRc.left = 20;
	qRc.right = 53 - 20;
	qRc.bottom = (GetG().m_scrHeight - 200) - 20;

	std::string dialog1 = "1234";
	debugBuffer.push_back(dialog1);
	dialog1 = "4567";
	debugBuffer.push_back(dialog1);
	nameBuffer = "PoolG";
	bufferPointer = debugBuffer.begin();
	namePointer = &nameBuffer;

	dlg_ON = false;
	startTalk = false;
	endTalk = false;

	V( D3DXCreateFont( GetG().m_dev, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &pFont) );
	V( D3DXCreateFont( GetG().m_dev, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("msgothic"), &qFont) );

	return hr;
}

HRESULT Dialog::release()
{
	dialog.release();
	name.release();

	SAFE_RELEASE( pFont );
	SAFE_RELEASE( qFont );
	
	return S_OK;
}


int EpWriteDialog( const char* c )
{
	Dialog::pFont->DrawTextA(0, c, -1, &Dialog::pRc, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );

	return 0;
} SCRIPT_CALLABLE_I_PC( EpWriteDialog )

START_SCRIPT_FACTORY(Dialog)
	CREATE_OBJ_COMMAND( EpWriteDialog )
END_SCRIPT_FACTORY(Dialog)

void Dialog::Toggle(bool *OK)
{
	if(	*OK == true )
		*OK = false;
	else
		*OK = true;
}

HRESULT Dialog::frameMove(double fTime, float fElapsedTime)
{
	dialog.frameMove(fElapsedTime);
	name.frameMove(fElapsedTime);
	
	return S_OK;
}

HRESULT Dialog::frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime)
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
	
	if( dlg_ON )
	{
		dialog.draw();
		name.draw();

		printDialog();
		printName();
		//GetScriptManager().execute("EpWorldState::printDialogText");

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
				Toggle(&dlg_ON);
				Toggle(&startTalk);
				break;
			case VK_RETURN:
				NextDialog();
				break;
		}
	}

	return S_OK;
}

void Dialog::printDialog()
{
	pFont->DrawTextA(0, bufferPointer->c_str(), -1, &pRc, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

void Dialog::printName()
{
	qFont->DrawTextA(0, namePointer->c_str(), -1, &qRc, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
}

void Dialog::NextDialog()
{
	if(dlg_ON && ((bufferPointer+1) != debugBuffer.end()))
		bufferPointer++;
	else
	{
		if ( dlg_ON && startTalk )
		{
			Toggle(&dlg_ON);
			Toggle(&startTalk);
			Toggle(&endTalk);
			bufferPointer = debugBuffer.begin();
		}
	}
}
