#include "EmptyProjectPCH.h"
#include "Utility.h"

Utility::Utility(void)
{
}

Utility::~Utility(void)
{
}

// 주어진 축에 대고 벡터를 각도만큼 돌린다.
void Utility::rotateAboutAxis( D3DXVECTOR3* pvOut, D3DXVECTOR3* pvAxis, float rad )
{
	D3DXMATRIX rotMat;
	D3DXVECTOR4 vOutTmp;

	// 축을 Normalize한다.
	D3DXVec3Normalize( pvAxis, pvAxis );

	// 회전 변환 행렬을 구한다.
	D3DXMatrixRotationAxis( &rotMat, pvAxis, rad );
	const D3DXMATRIX rotMatConst( rotMat );

	// 회전 변환을 적용한다.
	D3DXVec3Transform( &vOutTmp, pvOut, &rotMatConst );

	pvOut->x = vOutTmp.x;
	pvOut->y = vOutTmp.y;
	pvOut->z = vOutTmp.z;
}

float Utility::radBetweenVectors( const D3DXVECTOR3* v1, const D3DXVECTOR3* v2 )
{
	// A (dot product) B = abs(A) * abs(B) * cos(rad)

	float AdotB = D3DXVec3Dot( v1, v2 );

	float absA = D3DXVec3Length( v1 );
	float absB = D3DXVec3Length( v2 );

	return acos( AdotB / ( absA * absB ) );
}