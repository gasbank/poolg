#include "EmptyProjectPCH.h"
#include "Dialog.h"

Dialog::Dialog(void)
{
}

Dialog::~Dialog(void)
{
}

HRESULT Dialog::init()
{
	HRESULT hr = S_OK;

	rc.top = 0;
	rc.left = 0;
	rc.right = GetG().m_scrWidth;
	rc.bottom = GetG().m_scrHeight;

	OK = false;

	V( D3DXCreateFont( GetG().m_dev, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &pFont) );

	return hr;
}

void Dialog::release()
{
	SAFE_RELEASE( pFont );
}

void Dialog::print(WCHAR debugBuffer[256])
{
	pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

void Dialog::Toggle()
{
	if(	OK == true )
		OK = false;
	else
		OK = true;
}