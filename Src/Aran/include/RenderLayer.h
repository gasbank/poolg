#pragma once

class VideoMan;

class RenderLayer
{
public:
	RenderLayer(void);
	virtual ~RenderLayer(void);

	virtual HRESULT render() = 0;
	virtual HRESULT update(double fTime, float fElapsedTime) = 0;

	void setVideoMan( VideoMan* pVideoMan ) { m_pVideoMan = pVideoMan; }
	void setDev( LPDIRECT3DDEVICE9 pDev ) { m_lpDev = pDev; }
	void setVisible(BOOL b) { m_bVisible = b; }
	BOOL getVisible()		{ return m_bVisible; }
	
protected:
	VideoMan* m_pVideoMan;
	LPDIRECT3DDEVICE9 m_lpDev;
	BOOL m_bVisible;
};
