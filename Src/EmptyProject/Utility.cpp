#include "EmptyProjectPCH.h"
#include "Utility.h"

Utility::Utility(void)
{
}

Utility::~Utility(void)
{
}

// �־��� �࿡ ��� ���͸� ������ŭ ������.
void Utility::rotateAboutAxis( D3DXVECTOR3* pvOut, D3DXVECTOR3* pvAxis, float rad )
{
	D3DXMATRIX rotMat;
	D3DXVECTOR4 vOutTmp;

	// ���� Normalize�Ѵ�.
	D3DXVec3Normalize( pvAxis, pvAxis );

	// ȸ�� ��ȯ ����� ���Ѵ�.
	D3DXMatrixRotationAxis( &rotMat, pvAxis, rad );
	const D3DXMATRIX rotMatConst( rotMat );

	// ȸ�� ��ȯ�� �����Ѵ�.
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