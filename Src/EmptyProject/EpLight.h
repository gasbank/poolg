#pragma once

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
	void turnOnLight();
	void turnOffLight();
	void fadeInLight();
	void fadeOutLight();
	void flicker( float f ) { m_fFlickerDuration = f; }

private:
	void updateFadeColor( float fElapsedTime );
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
};