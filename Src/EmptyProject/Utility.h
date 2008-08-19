#pragma once

class Utility
{
public:
	Utility(void);
	~Utility(void);

	static void rotateAboutAxis( D3DXVECTOR3* vOut, D3DXVECTOR3* axis, float rad );

	static void printValue( const D3DXVECTOR3& vec3 )
	{
		printf( "(%.2f, %.2f, %.2f)", vec3.x, vec3.y, vec3.z );
	}
	static void printValue( const D3DXMATRIX& mat )
	{
		D3DXVECTOR3 vScale, vTrans, vRotAxis;
		D3DXQUATERNION qRot;
		float fRotAngle;
		D3DXMatrixDecompose( &vScale, &qRot, &vTrans, &mat );
		D3DXQuaternionToAxisAngle( &qRot, &vRotAxis, &fRotAngle );
		printf("Scale: "); printValue( vScale ); printf( "\n" );
		printf("Trans: "); printValue( vTrans ); printf( "\n" );
		printf("RotAxis: "); printValue( vRotAxis ); printf(" / Angle: %f\n", fRotAngle );
	}
};
