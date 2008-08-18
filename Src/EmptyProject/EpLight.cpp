#include "EmptyProjectPCH.h"
#include "EpLight.h"
#include "TopStateManager.h"
#include "World.h"

EpLight::EpLight(void)
{
	m_fFadeDuration	= 2.0f;
	m_fFadeTimer = 0.0f;
	m_fFadeTimerSign = 1.0f;
	m_fFlickerDuration = 0.0f;
	m_eLightState = LIGHT_NORMAL;
	m_fBrightness = 1.0f;
	m_bInFading = false;

	D3DXVECTOR3 dir( 0.0f, 0.0f, 1.0f );
	D3DXVECTOR3 pos( -38.0f, -10.0f, -40.0f );
	m_vDir = dir;
	m_vPos = pos;
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

	
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &m_vDir);
	m_light.Position = m_vPos;

	m_light.Falloff	= 1.0f; 
	m_light.Phi = D3DXToRadian(80);
	m_light.Theta = D3DXToRadian(10);

	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Range = 1000.0f;

	m_light.Attenuation0 = 1.0f;
	m_light.Attenuation1 = 0.01f;
	m_light.Attenuation2 = 0.0f;

	GetG().m_dev->SetLight(1, &m_light);
	GetG().m_dev->LightEnable( 1, FALSE );

	m_cAmbient = cvAmb;
	m_cDiffuse = cvDif;
	m_cSpecular = cvSpc;
}

LRESULT EpLight::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	D3DXCOLOR color( 1.0f, 0.0f, 0.0f, 1.0f );
	//static D3DXVECTOR3 vPos( -38.0f, -10.0f, 10.0f) ;
	switch ( uMsg )
	{
	case WM_KEYDOWN:
		switch( wParam )
		{
		case 'I':
			stopFlicker();
			fadeInLight();
			break;
		case 'O':
			fadeOutLight();
			break;
		case 'F':
			setFlickerColor( color );
			setFadeDuration( 0.5f );
			flicker( 10.0f );
			break;
	/*	case VK_UP:
			vPos.x += 1.0f;
			break;
		case VK_DOWN:
			vPos.x -= 1.0f;
			break;
		case VK_LEFT:
			vPos.y += 1.0f;
			break;
		case VK_RIGHT:
			vPos.y -= 1.0f;
			break;*/
		}
		break;
	}

	/*setLightPos(vPos);*/

	return S_OK;
}

void EpLight::frameMove( FLOAT fElapsedTime )
{
	// 스테이트는 기본적으로 LIGHT_NORMAL이고, 아래 과정에서 끝나지 않은 작업이 있으면 변할 수 있음.
	// 의도는 위와 같았으나 현재 기본으로 LIGHT_FADE 스테이트임.
	m_eLightState = LIGHT_NORMAL;

	updateFadeBrightness( fElapsedTime );
	updateFlicker( fElapsedTime );

	switch ( m_eLightState )
	{
	case LIGHT_FADE:
		m_light.Ambient = m_cAmbient * m_fBrightness;
		m_light.Diffuse = m_cDiffuse * m_fBrightness;
		m_light.Specular = m_cSpecular * m_fBrightness;
		break;
	case LIGHT_FLICKER:
		m_light.Ambient = m_cFlickerAmbient * m_fBrightness;
		m_light.Diffuse = m_cFlickerDiffuse * m_fBrightness;
		m_light.Specular = m_cFlickerSpecular * m_fBrightness;
		break;
	case LIGHT_NORMAL:
		m_light.Ambient = m_cAmbient;
		m_light.Diffuse = m_cDiffuse;
		m_light.Specular = m_cSpecular;
		break;
	}

	m_vDir = *GetG().m_camera.GetLookAtPt() - *GetG().m_camera.GetEyePt();
	m_vPos = *GetG().m_camera.GetEyePt();

	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &m_vDir);

	float vDirLen = D3DXVec3Length( (D3DXVECTOR3*)&m_light.Direction );
	if ( vDirLen < 1e-5 || vDirLen > 1e6 )
		m_light.Direction = DX_CONSTS::D3DXVEC3_Z;
	else
		m_light.Direction = m_vPos;

	m_light.Position = m_vPos;

	
	m_bLightValueDirty = true;

	if ( m_bLightValueDirty )
	{
		GetG().m_dev->SetLight( 1, &m_light );
		m_bLightValueDirty = false;
		//printf("lightPos: %f %f %f \n", m_light.Position.x, m_light.Position.y, m_light.Position.z );
	}
}

void EpLight::fadeInLight()
{
	turnOnLight();

	m_fFadeTimer += 0.01f;
	m_fFadeTimerSign = 1.0f;
}

void EpLight::fadeOutLight()
{
	if ( m_fFadeTimer > m_fFadeDuration)
		m_fFadeTimer = m_fFadeDuration - 0.01f;
	else
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

void EpLight::updateFadeBrightness( float fElapsedTime )
{
	m_bInFading = true;
	if ( 0.0f < m_fFadeTimer && m_fFadeTimer < m_fFadeDuration )
	{
		m_fFadeTimer += fElapsedTime * m_fFadeTimerSign;
	}
	else if ( m_fFadeTimer > m_fFadeDuration )
	{
		m_fFadeTimer = m_fFadeDuration;
		m_bInFading = false;
	}
	else if ( m_fFadeTimer < 0.0f )
	{
		m_fFadeTimer = 0.0f;
		m_bInFading = false;
	}

	m_eLightState = LIGHT_FADE;

	m_fBrightness = abs( sin( D3DXToRadian( m_fFadeTimer / m_fFadeDuration * 90.0f ) ) );

	//printf( "FadeTimer = %f \n", m_fFadeTimer );
	//printf( "Brightness = %f \n", m_fBrightness );
}

void EpLight::updateFlicker( float fElapsedTime )
{
	if ( m_fFlickerDuration > 0.0f )
	{
		m_fFlickerDuration -= fElapsedTime;

		if ( m_fFadeTimer == 0.0f )
			fadeInLight();
		else if ( m_fFadeTimer == m_fFadeDuration )
			fadeOutLight();

		m_eLightState = LIGHT_FLICKER;

	} else
		m_fFlickerDuration = 0.0f;
}

void EpLight::setColor( float r, float g, float b )
{
	m_cAmbient.r = r * 0.3f;
	m_cAmbient.g = g * 0.3f;
	m_cAmbient.b = b * 0.3f;

	m_cDiffuse.r = r * 0.6f;
	m_cDiffuse.g = g * 0.6f;
	m_cDiffuse.b = b * 0.6f;

	m_cSpecular.r = r * 0.1f;
	m_cSpecular.g = g * 0.1f;
	m_cSpecular.b = b * 0.1f;

	m_light.Ambient = m_cAmbient;
	m_light.Diffuse = m_cDiffuse;
	m_light.Specular = m_cSpecular;

	m_bLightValueDirty = true;
}

void EpLight::setFlickerColor( D3DXCOLOR& color )
{
	m_cFlickerAmbient = color * 0.3f;
	m_cFlickerDiffuse = color * 0.6f;
	m_cFlickerSpecular = color * 0.1f;
}

void EpLight::flicker( float flickerDuration )
{
	m_fFlickerDuration = flickerDuration;

	m_light.Ambient = m_cFlickerAmbient;
	m_light.Diffuse = m_cFlickerDiffuse;
	m_light.Specular = m_cFlickerSpecular;

	m_bLightValueDirty = true;
}
