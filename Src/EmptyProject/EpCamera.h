#pragma once

class EpCamera : public CFirstPersonCamera
{
public:
	EpCamera();

	void SetUpVector( D3DXVECTOR3* vecUp );

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
};
