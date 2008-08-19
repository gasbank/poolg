#pragma once
#include "EpCamera.h"
#include "EpLight.h"
#include "VideoMan.h"
#include "ScreenFlash.h"

class G : public Singleton<G>
{
public:
	G(void);
	~G(void);

	LPDIRECT3DDEVICE9	m_dev;
	EpCamera			m_camera;
	D3DLIGHT9			m_light;
	EpLight				m_EpLight;
	VideoMan			m_videoMan;
	ScreenFlash			m_screenFlash;

	
	int					m_scrWidth;
	int					m_scrHeight;
	D3DXMATRIX			g_orthoProjMat;
	D3DXMATRIX			g_fixedViewMat;
};
inline G& GetG() { return G::getSingleton(); }