#pragma once

#include "ArnNode.h"
class ArnIpo;

class ArnXformable : public ArnNode
{
public:
	virtual						~ArnXformable(void);

	const STRING&				getIpoName() const { return m_ipoName; }
	const D3DXMATRIX&			getFinalLocalXform();
	const D3DXVECTOR3&			getLocalXform_Scale() const { return m_localXform_Scale; }
	const D3DXQUATERNION&		getLocalXform_Rot() const { return m_localXform_Rot; }
	const D3DXVECTOR3&			getLocalXform_Trans() const { return m_localXform_Trans; }
	D3DXMATRIX					getFinalXform();
	bool						isAnimSeqEnded() const { return m_bAnimSeqEnded; }

	double						getAnimCtrlTime() const;
	void						setAnimCtrlTime( double dTime );
	void						setDoAnim( bool bDoAnim );
	void						resetAnimSeqTime() { m_bAnimSeqEnded = false; setAnimCtrlTime(0); }
	void						advanceTime( float fTime ) { m_d3dxAnimCtrl->AdvanceTime( fTime, 0 ); }
protected:
								ArnXformable(NODE_DATA_TYPE ndt);

	ArnIpo*						getIpo() const { return m_ipo; }
	void						setIpo(ArnIpo* val);
	void						setIpo(const STRING& ipoName);
	void						setIpoName(const char* ipoName) { m_ipoName = ipoName; }
	void						setLocalXform(const D3DXMATRIX& localXform);
	virtual void				update(double fTime, float fElapsedTime);

	// *** INTERNAL USE ONLY START ***
	void						configureAnimCtrl();
	// *** INTERNAL USE ONLY END ***
private:
	
	void						setAnimSeqEnded(bool val) { m_bAnimSeqEnded = val; }

	ArnIpo*						m_ipo;
	STRING						m_ipoName;

	D3DXMATRIX					m_localXform;
	D3DXMATRIX					m_localXformIpo;
	D3DXVECTOR3					m_localXform_Scale;
	D3DXQUATERNION				m_localXform_Rot;
	D3DXVECTOR3					m_localXform_Trans;

	D3DXMATRIX					m_animLocalXform;
	D3DXMATRIX					m_finalLocalXform;

	LPD3DXANIMATIONCONTROLLER	m_d3dxAnimCtrl;
	bool						m_bDoAnim;
	bool						m_bAnimSeqEnded;
	
};

