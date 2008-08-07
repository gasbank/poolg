#pragma once

class EpCamera : public CModelViewerCamera
{
public:
	EpCamera();

	//void SetViewParams3( D3DXVECTOR3* vecEye, D3DXVECTOR3* vecAt, D3DXVECTOR3* vecUp );
	void SetViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp );

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
};
