#include "EmptyProjectPCH.h"
#include "EpLight.h"

EpLight::EpLight(void)
{
	m_fFadeDuration	= 3.0f;
	m_fFadeTimer = 0.0f;
	m_fFadeTimerSign = 1.0f;
}

EpLight::~EpLight(void)
{
}

void EpLight::setupLight()
{
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	D3DCOLORVALUE cvAmb = { 0.3f, 0.3f, 0.3f, 1.0f };
	m_light.Ambient = cvAmb;
	D3DCOLORVALUE cvDif = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_light.Diffuse = cvDif;
	D3DCOLORVALUE cvSpc = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.Specular = cvSpc;

	D3DXVECTOR3 dir(10.0f, -10.0f, 10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &dir);

	D3DXVECTOR3 pos(-10.0f, 10.0f, -10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Position, &pos);

	m_light.Falloff = 0.5f; 
	m_light.Phi = D3DXToRadian(80);
	m_light.Theta = D3DXToRadian(10);

	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Range = 1000.0f;

	GetG().m_dev->SetLight(1, &m_light);
	GetG().m_dev->LightEnable( 1, FALSE );

	m_cAmbient = cvAmb;
	m_cDiffuse = cvDif;
	m_cSpecular = cvSpc;
}

LRESULT EpLight::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_KEYDOWN:
		switch( wParam )
		{
		case 'I':		
			fadeInLight();
			break;
		case 'O':
			fadeOutLight();
			break;
		}
		break;
	}

	return S_OK;
}

void EpLight::frameMove( FLOAT fElapsedTime )
{
	updateFadeColor( fElapsedTime );

	if ( m_bLightValueDirty )
		GetG().m_dev->SetLight( 1, &m_light );
}

void EpLight::fadeInLight()
{
	turnOnLight();

	m_fFadeTimer += 0.01f;
	m_fFadeTimerSign = 1.0f;
}

void EpLight::fadeOutLight()
{
	m_fFadeTimer -= 0.01f;
	m_fFadeTimerSign = -1.0f;
}

void EpLight::turnOnLight()
{
	GetG().m_dev->LightEnable( 1, TRUE );
}

void EpLight::turnOffLight()
{
	GetG().m_dev->LightEnable( 1, FALSE );
}

void EpLight::updateFadeColor( float fElapsedTime )
{
	if ( 0.0f < m_fFadeTimer && m_fFadeTimer < m_fFadeDuration )
		m_fFadeTimer += fElapsedTime * m_fFadeTimerSign;
	else if ( m_fFadeTimer > m_fFadeDuration )
		m_fFadeTimer = m_fFadeDuration;
	else if ( m_fFadeTimer < 0.0f )
		m_fFadeTimer = 0.0f;

	float fBrightness = abs( sin( D3DXToRadian( 
		m_fFadeTimer / m_fFadeDuration * 90.0f ) ) );

	m_light.Ambient = m_cAmbient * fBrightness;
	m_light.Diffuse = m_cDiffuse * fBrightness;
	m_light.Specular = m_cSpecular * fBrightness;

	m_bLightValueDirty = true;
}