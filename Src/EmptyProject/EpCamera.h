﻿#pragma once

#include "ArnCamera.h"

enum RunningCamera { 
	CAMERA_NORMAL, 
	CAMERA_SMOOTH, 
	CAMERA_EXTERNAL, 
	CAMERA_ATTACH, 
	CAMERA_SMOOTH_ATTACH 
};



/**
@brief 게임 전반에 쓰이는 카메라 조작 클래스

캐릭터를 쫓아다니는 카메라, 적군을 만났을 때 카메라웍이 어떻게 되는지 등을
구현해 놓은 클래스입니다. 또한 카메라 흔들리기 기능도 있습니다.

아래는 카메라 모드에 대한 목록입니다.

- CAMERA_NORMAL : SetViewParamsWithUp()에 의해 지정된 위치로 카메라를 옮긴다.
- CAMERA_SMOOTH : 이전 위치로부터 SetDesViewParams()에 의해 지정된 위치로 카메라를 서서히 옮긴다.
                움직이는데 걸리는 시간도 지정할 수 있다.
- CAMERA_EXTERNAL : 외부에서 들어온 ArnCamera에 의해 카메라 위치가 정해진다.
- CAMERA_SMOOTH_ATTACH : 이전 위치로부터 setAttachPos에 의해 지정된 포인터가 가리키는 벡터를 z축으로
                       -30만큼 떨어진 곳에서 바라보도록 서서이 붙어서 따라다닌다.
*/
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
	void setExternalCamera( ArnCamera* arnCam ) { _ASSERTE( arnCam && "Camera not found" ); m_pArnCam = arnCam; }
	void beginShoulderLookCamera( const D3DXVECTOR3* pvMePos, const D3DXVECTOR3* pvOppPos );
	void begin( RunningCamera rc );
	D3DXVECTOR3* GetUpPt();
	bool getShake() const { return m_bShake; }
	void setShake(bool val) { m_bShake = val; }
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
	void pulledEye( D3DXVECTOR3* vPulledEye, D3DXVECTOR3* vLookAt, D3DXVECTOR3* vEye, int nth );
	void processShake( float fElapsedTime );

	D3DXVECTOR3		m_vUp;
	ArnCamera*		m_pArnCam;
	RunningCamera	m_runningCamera;

	D3DXVECTOR3		m_vPrevEye;
	D3DXVECTOR3		m_vPrevLookAt;
	D3DXVECTOR3		m_vPrevUp;

	D3DXVECTOR3		m_vDesEye;
	D3DXVECTOR3		m_vDesLookAt;
	D3DXVECTOR3		m_vDesUp;

	D3DXVECTOR3		m_vEyeShake, m_vLookAtShake, m_vUpShake;

	const D3DXVECTOR3*	m_vPos;

	float			m_fSmoothCameraTimer;
	float			m_fSmoothCameraDuration;

	bool			m_bViewParamsDirty;
	bool			m_bCamManualMovement;
	bool			m_bUpdateContinue;

	bool			m_bShake;
	float			m_nextShakeTime;	

	float			m_minPullDist;
};