#pragma once

#include "Picture.h"
#include "TileManager.h"


/**
@brief 캐릭터 사이의 대화를 관리하는 클래스

스크립트에 정의된 대화를 읽어와서 하나의 완전한 대화를 구성하는 클래스입니다.
이 대화는 이벤트 시스템에 의해서 시작될 수 있습니다.
*/
class Dialog
{
public:
	struct Speak
	{
		const char* name;		// Speaker name
		const char* content;
	};
	
	static Dialog* createDialogByScript( const char* dialogName );
	~Dialog(void);
	HRESULT init();
	HRESULT release();

	void printDialog();
	void printName();
	bool nextDialog();

	HRESULT frameMove (double fTime, float fElapsedTime);
	HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double dTime, float fElapsedTime);
	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	const TileRegion& getRegion() const { return m_region; }
	bool isOneTime() const { return m_bOneTime; }
	bool isTalking() const { return m_bTalking; }
	void setOneTime(bool b) { m_bOneTime = b; }
	const std::string& getDialogName() const { return m_dialogName; }
	bool getRemoveFlag() const { return m_bRemoveFlag; }
	void setRemoveFlag(bool val) { m_bRemoveFlag = val; }

	void updateDialogPosition();

	void printDebugInfo() const;
private:
	Dialog(void);
	Dialog( Speak* speakArray, UINT speakCount, const TileRegion* region, bool bOneTime, const char* dialogName );

	void ctorDialogPane();
	

	Picture m_contentPic;
	Picture m_namePic;

	Speak* m_speakArray;
	UINT m_speakCount;
	UINT m_curSpeakIdx;
	TileRegion m_region; // Tile region which a player can start this dialog session with
	bool m_bOneTime;
	bool m_bTalking;
	
	RECT m_contentRect;
	RECT m_nameRect;

	std::string m_dialogName;

	bool m_bInit;
	bool m_bRemoveFlag;
	
};