#pragma once

class EpLight
{
public:
	EpLight(void);
	~EpLight(void);

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void frameMove( FLOAT fElapsedTime );

	void setupLight();
	void turnOnLight();
	void turnOffLight();
	void fadeInLight();
	void fadeOutLight();

private:
	void updateFadeColor( float fElapsedTime );

	D3DLIGHT9			m_light;
	float				m_fFadeTimer;
	float				m_fFadeDuration;
	D3DXCOLOR			m_cAmbient;
	D3DXCOLOR			m_cDiffuse;
	D3DXCOLOR			m_cSpecular;
	bool				m_bLightValueDirty;
	float				m_fFadeTimerSign;
};