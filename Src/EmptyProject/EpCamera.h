#pragma once

#include "ArnCamera.h"

enum RunningCamera { CAMERA_NORMAL, CAMERA_EXTERNAL };

class EpCamera : public CModelViewerCamera
{
public:
	EpCamera();

	virtual void FrameMove( FLOAT fElapsedTime );
	void SetViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp );
	D3DXVECTOR3* GetUpPt();
	void beginExternalCamera( ArnCamera* arnCam );

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
	void updateExternalCamera( ArnCamera* arnCam );

	D3DXVECTOR3		m_vUp;
	ArnCamera*		m_pArnCam;
	RunningCamera	m_runningCamera;
};
