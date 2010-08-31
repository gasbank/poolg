#pragma once
#include "EpCamera.h"
#include "ScreenFlash.h"

class VideoManDx9;

/**
	@brief EmptyProject 프로그램 전역으로 쓰이는 변수를 모아놓은 클래스

	Singleton으로 구현되어 있습니다.
*/
class G : public Singleton<G>
{
public:
	G(void);
	~G(void);

	// Wow! G::m_dev disappears and goes into the history of PoolG project.
	// Thank a lot. It was big mistake to make you global.
	//LPDIRECT3DDEVICE9	m_dev;

	EpCamera			m_camera;			///< 전역적으로 쓰이는 카메라
	ArnLightData		m_light;			///< 전역적으로 쓰이는 광원
	VideoManDx9*		m_videoMan;			///< Aran 모델을 렌더링 하기 위해 필요한 클래스
	ScreenFlash			m_screenFlash;		///< 화면을 번쩍이게 해 주는 일을 담당하는 클래스

	int					m_scrWidth;			///< 가로 해상도
	int					m_scrHeight;		///< 세로 해상도
	ArnMatrix			g_orthoProjMat;		///< Orthogonal Projection Matrix (for GUI purpose)
	ArnMatrix			g_fixedViewMat;		///< Fixed View Matrix (for GUI purpose)
};
inline G& GetG() { return G::getSingleton(); }