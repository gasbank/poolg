#pragma once

#include "ArnCamera.h"

enum RunningCamera { 
	CAMERA_NORMAL, 
	CAMERA_SMOOTH, 
	CAMERA_EXTERNAL, 
	CAMERA_ATTACH, 
	CAMERA_SMOOTH_ATTACH 
};

// CAMERA_NORMAL : SetViewParamsWithUp()에 의해 지정된 위치로 카메라를 옮긴다.
// CAMERA_SMOOTH : 이전 위치로부터 SetDesViewParams()에 의해 지정된 위치로 카메라를 서서히 옮긴다.
//				움직이는데 걸리는 시간도 지정할 수 있다.
// CAMERA_EXTERNAL : 외부에서 들어온 ArnCamera에 의해 카메라 위치가 정해진다.
// CAMERA_SMOOTH_ATTACH : 이전 위치로부터 setAttachPos에 의해 지정된 포인터가 가리키는 벡터를 z축으로
//				-30만큼 떨어진 곳에서 바라보도록 서서이 붙어서 따라다닌다.

class EpCamera : public CModelViewerCamera
{
public:
	EpCamera();

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void frameMove( FLOAT fElapsedTime );
	void setViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp );
	void setViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, D3DXVECTOR3* pvUp );
	void setDesViewParams( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPtg, D3DXVECTOR3* vUp );
	void setAttachPos( const D3DXVECTOR3* pos ) { m_vPos = pos; }
	void setSmoothCameraDuration( float f ) { m_fSmoothCameraDuration = f; }
	void setExternalCamera( ArnCamera* arnCam ) { assert( arnCam ); m_pArnCam = arnCam; }
	void beginShoulderLookCamera( const D3DXVECTOR3* pvMePos, const D3DXVECTOR3* pvOppPos );
	void begin( RunningCamera rc );
	D3DXVECTOR3* GetUpPt();

protected:
	virtual D3DUtil_CameraKeys MapKey( UINT nKey );
	void updateSmoothCamera( float fElapsedTime );
	void updateExternalCamera( ArnCamera* arnCam );
	void updateSmoothAttachCamera( float fElapsedTime );
	void updateAttachCamera();
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

	const D3DXVECTOR3*	m_vPos;

	float			m_fSmoothCameraTimer;
	float			m_fSmoothCameraDuration;

	bool			m_bViewParamsDirty;
	bool			m_bCamManualMovement;
	bool			m_bUpdateContinue;
};