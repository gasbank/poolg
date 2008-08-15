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

	void printDialog();
	void printName();
	bool nextDialog();

	HRESULT frameMove (double fTime, float fElapsedTime);
	HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime);
	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	const RECT& getRegion() const { return m_region; }
	bool isOneTime() const { return m_bOneTime; }
	bool isTalking() const { return m_bTalking; }
	void setOneTime(bool b) { m_bOneTime = b; }
	const std::string& getDialogName() const { return m_dialogName; }

private:
	void ctorDialogPane();
	Dialog( Speak* speakArray, UINT speakCount, const RECT& region, bool bOneTime, const char* dialogName );

	Picture m_contentPic;
	Picture m_namePic;

	Speak* m_speakArray;
	UINT m_speakCount;
	UINT m_curSpeakIdx;
	RECT m_region; // Tile region which a player can start this dialog session with
	bool m_bOneTime;
	bool m_bTalking;
	
	LPD3DXFONT m_nameFont;
	LPD3DXFONT m_contentFont;
	RECT m_contentRect;
	RECT m_nameRect;

	std::string m_dialogName;
};