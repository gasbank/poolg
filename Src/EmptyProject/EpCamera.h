#pragma once

class EpCamera : public CFirstPersonCamera
{
public:
	EpCamera();

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
};
