#pragma once

class ArnMath
{
public:
	static D3DXVECTOR3		QuatToEuler(const D3DXQUATERNION* quat);
	static D3DXQUATERNION	EulerToQuat(const D3DXVECTOR3* vec3);
	static D3DXVECTOR3		Vec3RadToDeg(const D3DXVECTOR3* vec3);
	static DWORD			Float4ColorToDword(const D3DCOLORVALUE* cv);
private:
	ArnMath(void);
	~ArnMath(void);
};
