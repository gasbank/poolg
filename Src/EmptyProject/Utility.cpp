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
