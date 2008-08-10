#pragma once

#include "Picture.h"

typedef std::vector<std::string> StringVector;

class Dialog
{
public:
	struct Speak
	{
		const char* name;		// Speaker name
		const char* content;
	};
	Dialog(void);
	static Dialog* createDialogByScript( const char* dialogName );
	~Dialog(void);
	HRESULT init();
	HRESULT release();
	void Toggle(bool *OK);
	void printDialog();
	void printName();
	void NextDialog();

	HRESULT frameMove (double fTime, float fElapsedTime);
	HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime);
	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	const RECT& getRegion() const { return m_region; }
	bool isOneTime() const { return m_bOneTime; }
	void setOneTime(bool b) { m_bOneTime = b; }

	static LPD3DXFONT pFont;
	static LPD3DXFONT qFont;
	static RECT pRc;
	static RECT qRc;

	bool dlg_ON;
	bool startTalk;
	bool endTalk;

private:
	void ctorDialogPane();
	Dialog( Speak* speakArray, UINT speakCount, const RECT& region, bool bOneTime );

	Picture dialog;
	Picture name;

	Speak* m_speakArray;
	UINT m_speakCount;
	UINT m_curSpeakIdx;
	RECT m_region; // Tile region which a player can start this dialog session with
	bool m_bOneTime;
	
};