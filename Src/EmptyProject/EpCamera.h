#pragma once

class EpCamera : public CModelViewerCamera
{
public:
	EpCamera();

	//void SetViewParams3( D3DXVECTOR3* vecEye, D3DXVECTOR3* vecAt, D3DXVECTOR3* vecUp );

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
};
