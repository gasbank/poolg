#pragma once

class EpCamera : public CModelViewerCamera
{
public:
	EpCamera();

	void SetViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp );
	D3DXVECTOR3* GetUpPt();

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );

	D3DXVECTOR3 m_vUp;
};
