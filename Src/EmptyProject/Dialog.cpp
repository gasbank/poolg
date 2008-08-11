#include "EmptyProjectPCH.h"
#include "Dialog.h"
#include "ScriptManager.h"

LPD3DXFONT	Dialog::pFont = 0;
RECT		Dialog::pRc;
LPD3DXFONT	Dialog::qFont = 0;
RECT		Dialog::qRc;

Dialog::Dialog(void)
{
	ctorDialogPane();

	m_speakArray = 0;
	m_speakCount = 0;
	m_curSpeakIdx = 0;
}

Dialog::Dialog( Speak* speakArray, UINT speakCount, const RECT& region, bool bOneTime )
{
	ctorDialogPane();
	m_speakArray	= speakArray;
	m_speakCount	= speakCount;
	m_curSpeakIdx	= 0;
	m_region		= region;
	m_bOneTime		= bOneTime;
}
Dialog::~Dialog(void)
{
	
}



HRESULT Dialog::init()
{
	HRESULT hr = S_OK;

	const UINT dialogPaneM_H = 200;
	const UINT dialogPaneM_W = 200;
	const UINT dialogPaneN_H = 60;
	const UINT dialogPaneN_W = 150;
	const UINT dialogPaneM_S = 20;
	const UINT dialogPaneN_S = 16;

	pRc.top = GetG().m_scrHeight - dialogPaneM_H + dialogPaneM_S;
	pRc.left = dialogPaneM_S;
	pRc.right = dialogPaneM_W - dialogPaneM_S;
	pRc.bottom = GetG().m_scrHeight - dialogPaneM_S;
	qRc.top = (GetG().m_scrHeight - dialogPaneM_H) - dialogPaneN_H + dialogPaneN_S;
	qRc.left = dialogPaneN_S;
	qRc.right = dialogPaneN_W - dialogPaneN_S;
	qRc.bottom = (GetG().m_scrHeight - dialogPaneM_H) - dialogPaneN_S;

	
	dlg_ON = false;
	startTalk = false;
	endTalk = false;

	if ( !pFont )
		V( D3DXCreateFont( GetG().m_dev, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &pFont) );
	if ( !qFont )
		V( D3DXCreateFont( GetG().m_dev, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("msgothic"), &qFont) );

	return hr;
}

HRESULT Dialog::release()
{
	dialog.release();
	name.release();

	SAFE_RELEASE( pFont );
	SAFE_RELEASE( qFont );
	SAFE_DELETE_ARRAY( m_speakArray );

	return S_OK;
}

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
	GetG().m_dev->SetRenderState(D3DRS_LIGHTING, FALSE);
	GetG().m_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GetG().m_dev->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	GetG().m_dev->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

	D3DMATERIAL9 material;
	D3DCOLORVALUE cv, cv2;
	cv.a = 0.3f; cv.r = 0.1f; cv.g = 0.1f; cv.b = 0.1f;
	cv2.a = 0.3f; cv2.r = 1.0f; cv2.g = 0.1f; cv2.b = 0.3f;
	material.Ambient = cv;
	material.Diffuse = cv2;
	material.Emissive = cv;
	material.Power = 1.0f;
	material.Specular = cv2;
	GetG().m_dev->SetMaterial(&material);

	//drawFixedText(scrWidth, scrHeight);

	GetG().m_dev->SetRenderState(D3DRS_ZENABLE, FALSE);
	
	if( dlg_ON )
	{
		D3DPERF_BeginEvent(0x12345678, L"Draw Dialog Pane");
		dialog.draw();
		name.draw();
		D3DPERF_EndEvent();

		printDialog();
		printName();

	}

	GetG().m_dev->SetRenderState(D3DRS_ZENABLE, TRUE);
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
	pFont->DrawTextA(0, m_speakArray[m_curSpeakIdx].content, -1, &pRc, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

void Dialog::printName()
{
	qFont->DrawTextA(0, m_speakArray[m_curSpeakIdx].name, -1, &qRc, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
}

void Dialog::NextDialog()
{
	if(dlg_ON && (m_curSpeakIdx < m_speakCount - 1))
		m_curSpeakIdx++;
	else
	{
		if ( dlg_ON && startTalk )
		{
			Toggle(&dlg_ON);
			Toggle(&startTalk);
			Toggle(&endTalk);
			m_curSpeakIdx = 0;
		}
	}
}

Dialog* Dialog::createDialogByScript( const char* dialogName )
{
	char tempBuf[256];
	Tcl_Interp* interp = GetScriptManager().getInterp();
	
	StringCchPrintfA( tempBuf, 256, "%s::region", dialogName );
	RECT region;
	GetScriptManager().readRect( tempBuf, region );

	StringCchPrintfA( tempBuf, 256, "%s::dialog", dialogName );
	Tcl_Obj* dialogObj = GetScriptManager().getObject( tempBuf );
	int dialogTokenCount;
	Tcl_ListObjLength( interp, dialogObj, &dialogTokenCount );
	UINT speakCount = dialogTokenCount / 2;
	Dialog::Speak* speakArray = new Dialog::Speak[ speakCount ];
	UINT i;
	for ( i = 0; i < speakCount; ++i )
	{
		Tcl_Obj* elem;
		int length;
		Tcl_ListObjIndex( interp, dialogObj, i*2 + 0, &elem );
		speakArray[ i ].name = Tcl_GetStringFromObj( elem, &length );
		Tcl_ListObjIndex( interp, dialogObj, i*2 + 1, &elem );
		speakArray[ i ].content = Tcl_GetStringFromObj( elem, &length );
	}

	StringCchPrintfA( tempBuf, 256, "%s::oneTime", dialogName );
	int oneTime = GetScriptManager().readInt( tempBuf );
	return new Dialog( speakArray, speakCount, region, oneTime?true:false );
}

void Dialog::ctorDialogPane()
{
	dialog.init(L"Images/dae-sa.png", GetG().m_dev);
	dialog.setPos (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f), 2.8f);
	dialog.setSize(200, 200);
	name.init(L"Images/name_window.png", GetG().m_dev);
	name.setPos (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f) + 200, 2.8f);
	name.setSize(150, 60);
}