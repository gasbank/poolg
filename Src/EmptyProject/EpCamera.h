#pragma once

class EpCamera : public CFirstPersonCamera
{
public:
	EpCamera();

	void move( D3DVECTOR* posA, D3DVECTOR* posB, float duration );

protected:
	virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
};
