#pragma once

enum LightState { LIGHT_FADE, LIGHT_FLICKER, LIGHT_NORMAL };

class EpLight
{
public:
	EpLight(void);
	~EpLight(void);

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void frameMove( FLOAT fElapsedTime );

	void setFadeDuration( float f ) { m_fFadeDuration = f; }
	void setupLight();
	void setColor( float r, float g, float b );
	void setFlickerColor( D3DXCOLOR& color );
	void setLightPos( D3DXVECTOR3& pos ) { m_light.Position = pos; m_bLightValueDirty = true; }
	void setLightDir( D3DXVECTOR3& dir ) { m_light.Direction = dir; m_bLightValueDirty = true; }
	void turnOnLight();
	void turnOffLight();
	void fadeInLight();
	void fadeOutLight();
	void flicker( float f );
	void stopFlicker() { m_fFlickerDuration = 0.0f; }

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

	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vPos;
};