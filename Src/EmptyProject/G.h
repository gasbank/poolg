#pragma once
#include "EpCamera.h"
#include "VideoMan.h"

class G : public Singleton<G>
{
public:
	G(void);
	~G(void);

	LPDIRECT3DDEVICE9	m_dev;
	EpCamera			m_camera;
	D3DLIGHT9			m_light;
	VideoMan			m_videoMan;

	
	int					m_scrWidth;
	int					m_scrHeight;
};
inline G& GetG() { return G::getSingleton(); }