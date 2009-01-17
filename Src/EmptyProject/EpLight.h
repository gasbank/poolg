#pragma once

enum LightState { LIGHT_FADE, LIGHT_FLICKER, LIGHT_NORMAL };


/**
@brief 게임 전반에 쓰이는 광원 조작 클래스

페이드 인, 아웃이나 빛의 색 변경 등을 조작합니다.
빛이 깜빡깜빡이는 flicker도 지원합니다.
*/
class EpLight : public Singleton<EpLight>
{
public:
	EpLight(void);
	~EpLight(void);

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void frameMove( FLOAT fElapsedTime );
	virtual void frameRender( IDirect3DDevice9* pd3dDevice );

	bool isInFading() const { return m_bInFading; }

	void setFadeDuration( float f ) { m_fFadeDuration = f; }
	void setupLight( IDirect3DDevice9* pd3dDevice );
	void setColor( float r, float g, float b );
	void setFlickerColor( D3DXCOLOR& color );
	void setLightPos( D3DXVECTOR3& pos ) { m_light.Position = pos; m_bLightValueDirty = true; }
	void setBrightness( float b ) { m_fBrightness = b; }
	void turnOnLight();
	void turnOffLight();
	void fadeInLight();
	void fadeInLightForcedDelayed( float d );
	void fadeOutLight();
	void flicker( float flickerDuration );
	void stopFlicker();
	bool isFlicker() { return m_bIsFlicker; }

	void setLight( D3DLIGHT9* light );;
private:
	void updateFadeBrightness( float fElapsedTime );
	void updateFlicker( float fElapsedTime );

	D3DLIGHT9			m_light;
	float				m_fFadeTimer;
	
	float				m_fFadeDuration;
	
	D3DXCOLOR			m_cAmbient;
	D3DXCOLOR			m_cDiffuse;
	D3DXCOLOR			m_cSpecular;
	bool				m_bLightValueDirty;
	float				m_fFadeTimerSign;
	float				m_fFlickerDuration;
	D3DXCOLOR			m_cFlickerAmbient;
	D3DXCOLOR			m_cFlickerDiffuse;
	D3DXCOLOR			m_cFlickerSpecular;
	float				m_fBrightness;
	LightState			m_eLightState;

	bool				m_bInFading;

	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vPos;

	float				m_fDelay;

	bool				m_bIsFlicker;
	bool				m_bLightOn;
};

inline EpLight& GetEpLight() { return EpLight::getSingleton(); }

SCRIPT_FACTORY( EpLight )