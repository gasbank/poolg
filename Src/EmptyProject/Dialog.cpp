#include "EmptyProjectPCH.h"
#include "Dialog.h"
#include "ScriptManager.h"
#include "Utility.h"
#include "Sprite.h"
#include "SpriteManager.h"

extern LPD3DXFONT						g_dlgNameFont;
extern LPD3DXFONT						g_dlgContentFont;


Dialog::Dialog(void)
{
	ctorDialogPane();
}

Dialog::Dialog( Speak* speakArray, UINT speakCount, const TileRegion* region, bool bOneTime, const char* dialogName )
{
	ctorDialogPane();

	m_speakArray	= speakArray;
	m_speakCount	= speakCount;
	m_region		= *region;
	m_bOneTime		= bOneTime;
	m_dialogName	= dialogName;
}
void Dialog::ctorDialogPane()
{
	// TODO Dialog pane images
	/*m_contentPic.init(L"Images/dae-sa.png", GetG().m_dev);
	m_contentPic.setPos (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f), 2.8f);
	m_contentPic.setSize(200, 200);
	m_namePic.init(L"Images/name_window.png", GetG().m_dev);
	m_namePic.setPos (-(GetG().m_scrWidth / 2.0f), -(GetG().m_scrHeight / 2.0f) + 200, 2.8f);
	m_namePic.setSize(150, 60);*/

	m_speakArray	= 0;
	m_speakCount	= 0;
	m_curSpeakIdx	= 0;
	ZeroMemory( &m_region, sizeof( RECT ) );
	m_bOneTime		= true;
	m_bInit			= false;
	m_bRemoveFlag	= false;
}

Dialog::~Dialog(void)
{
	
}

HRESULT Dialog::init()
{
	if ( m_bInit )
		throw std::runtime_error( "Double init error" );
	HRESULT hr = S_OK;

	updateDialogPosition();


	
	m_bTalking = false;


	m_bInit = true;
	return hr;
}

HRESULT Dialog::release()
{
	m_contentPic.release();
	m_namePic.release();

	SAFE_DELETE_ARRAY( m_speakArray );

	m_bInit = false;

	return S_OK;
}

HRESULT Dialog::frameMove(double fTime, float fElapsedTime)
{
	if ( m_bInit )
	{
		m_contentPic.frameMove(fElapsedTime);
		m_namePic.frameMove(fElapsedTime);
	}
	return S_OK;
}

HRESULT Dialog::frameRender( IDirect3DDevice9* pd3dDevice,  double dTime, float fElapsedTime )
{
	if ( m_bInit )
	{
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pd3dDevice->SetTransform(D3DTS_VIEW, GetG().g_fixedViewMat.getConstDxPtr());
		pd3dDevice->SetTransform(D3DTS_PROJECTION, GetG().g_orthoProjMat.getConstDxPtr());

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

		//drawFixedText(scrWidth, scrHeight);

		pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		if( m_bTalking )
		{
			D3DPERF_BeginEvent(0x12345678, L"Draw Dialog Pane");
			//m_contentPic.draw();
			//m_namePic.draw();
			D3DPERF_EndEvent();

			printDialog();
			printName();

		}

		pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	
	return S_OK;
}

HRESULT Dialog::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_KEYDOWN )
	{
		switch ( wParam )
		{
			case VK_RETURN:
				
				break;
		}
	}

	return S_OK;
}

void Dialog::printDialog()
{
	g_dlgNameFont->DrawTextA(0, m_speakArray[m_curSpeakIdx].content, -1, &m_contentRect, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

void Dialog::printName()
{
	g_dlgContentFont->DrawTextA(0, m_speakArray[m_curSpeakIdx].name, -1, &m_nameRect, DT_NOCLIP | DT_WORDBREAK, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
}

bool Dialog::nextDialog()
{
	if ( m_bTalking == true )
	{
		m_curSpeakIdx++;
		if ( m_curSpeakIdx >= m_speakCount )
		{
			m_bTalking = false;
			if ( isOneTime() )
			{
				return false;
			}
		}
	}
	else
	{
		m_bTalking = true;
		m_curSpeakIdx = 0;
	}

	return true;
}

Dialog* Dialog::createDialogByScript( const char* dialogName )
{
	char tempBuf[256];
	Tcl_Interp* interp = GetScriptManager().getInterp();
	
	StringCchPrintfA( tempBuf, 256, "%s::region", dialogName );
	TileRegion region;
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

	return new Dialog( speakArray, speakCount, &region, oneTime?true:false, dialogName );
}

void Dialog::printDebugInfo() const
{
	printf( "Dlg: %-30s spkCount=%d curSpkIdx=%d", getDialogName().c_str(), m_speakCount, m_curSpeakIdx );
	printf( " region=" ); Utility::printValue( m_region );
	printf( " onetime=%d talking=%d", m_bOneTime, m_bTalking );
	printf( " init=%d remove=%d\n", m_bInit, m_bRemoveFlag );
}

void Dialog::updateDialogPosition()
{
	const UINT dialogPaneM_H = 200;
	const UINT dialogPaneM_W = 200;
	const UINT dialogPaneN_H = 60;
	const UINT dialogPaneN_W = 150;
	const UINT dialogPaneM_S = 20;
	const UINT dialogPaneN_S = 16;

	m_contentRect.top = GetG().m_scrHeight - dialogPaneM_H + dialogPaneM_S;
	m_contentRect.left = dialogPaneM_S;
	m_contentRect.right = dialogPaneM_W - dialogPaneM_S;
	m_contentRect.bottom = GetG().m_scrHeight - dialogPaneM_S;
	m_nameRect.top = (GetG().m_scrHeight - dialogPaneM_H) - dialogPaneN_H + dialogPaneN_S;
	m_nameRect.left = dialogPaneN_S;
	m_nameRect.right = dialogPaneN_W - dialogPaneN_S;
	m_nameRect.bottom = (GetG().m_scrHeight - dialogPaneM_H) - dialogPaneN_S;
}