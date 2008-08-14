#pragma once

#include "ArnCamera.h"

enum RunningCamera { CAMERA_NORMAL, CAMERA_SMOOTH, CAMERA_EXTERNAL };

// CAMERA_NORMAL : SetViewParamsWithUp()�� ���� ������ ��ġ�� ī�޶� �ű��.
// CAMERA_SMOOTH : ���� ��ġ�κ��� SetDesViewParams()�� ���� ������ ��ġ�� ī�޶� ������ �ű��.
//					�����̴µ� �ɸ��� �ð��� ������ �� �ִ�.
// CAMERA_EXTERNAL : �ܺο��� ���� ArnCamera�� ���� ī�޶� ��ġ�� ��������.

class EpCamera : public CModelViewerCamera
{
public:
	EpCamera();

	virtual void frameMove( FLOAT fElapsedTime );
	void setViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp );
	void setDesViewParams( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp );
	void setSmoothCameraDuration( float f ) { m_fSmoothCameraDuration = f; }
	void setExternalCamera( ArnCamera* arnCam ) { m_pArnCam = arnCam; }
	void begin( RunningCamera rc );
	D3DXVECTOR3* GetUpPt();

protected:
	virtual D3DUtil_CameraKeys MapKey( UINT nKey );
	void updateSmoothCamera( float fElapsedTime );
	void updateExternalCamera( ArnCamera* arnCam );
	void lerpViewParams( 
		D3DXVECTOR3* pvEyeOut, D3DXVECTOR3* pvLookAtOut, D3DXVECTOR3* pvUpOut, 
		D3DXVECTOR3* pvEye1, D3DXVECTOR3* pvLookAt1, D3DXVECTOR3* pvUp1,
		D3DXVECTOR3* pvEye2, D3DXVECTOR3* pvLookAt2, D3DXVECTOR3* pvUp2,
		float s	);

	D3DXVECTOR3		m_vUp;
	ArnCamera*		m_pArnCam;
	RunningCamera	m_runningCamera;

	D3DXVECTOR3		m_vPrevEye;
	D3DXVECTOR3		m_vPrevLookAt;
	D3DXVECTOR3		m_vPrevUp;

	D3DXVECTOR3		m_vDesEye;
	D3DXVECTOR3		m_vDesLookAt;
	D3DXVECTOR3		m_vDesUp;

	float			m_fSmoothCameraTimer;
	float			m_fSmoothCameraDuration;
};