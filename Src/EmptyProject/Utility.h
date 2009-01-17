#pragma once

#include "TileManager.h"


/**
@brief 귀찮아서 한번에 모아둔 각종 함수들 -_-

충돌 테스트를 위한 레이 테스팅 관련 함수 및 디버그용 프린트 함수가 모여있습니다.
*/
class Utility
{
public:
	Utility(void);
	~Utility(void);

	static void rotateAboutAxis( D3DXVECTOR3* vOut, D3DXVECTOR3* axis, float rad );
	static float radBetweenVectors( const D3DXVECTOR3* v1, const D3DXVECTOR3* v2 );

	// If a ray intersects a triangle within distMin, true is returned.
	// Otherwise false is returned.
	static bool FullTraverseExhaustiveRayTesting( 
		ArnNode* node, const D3DXVECTOR3& rayStartPos, const D3DXVECTOR3& rayDir, float distMin );
	
	// Return distance to the nth closest mesh
	static float FullTraverseExhaustiveRayTesting( 
		ArnNode* node, const D3DXVECTOR3& rayStartPos, const D3DXVECTOR3& rayDir, int nth );

	// return the number of intersected meshes
	static int	FullTraverseExhaustiveRayTesting( 
		ArnNode* node, const D3DXVECTOR3& rayStartPos, const D3DXVECTOR3& rayDir );

	// Ultimate version of FullTraverseExhaustiveRayTesting
	static void Utility::FullTraverseExhaustiveRayTesting( 
		ArnNode* node, 
		const D3DXVECTOR3& rayStartPos, 
		const D3DXVECTOR3& rayDir,
		int nth, 
		float* pNthClosestDist, 
		int* pIntersectedMeshCount
		);

	//// Return the number of intersected meshes between two position
	//static int intersectionsBetweenTwoPosition( 
	//	const D3DXVECTOR3* v1, const D3DXVECTOR3* v2, ArnNode* arnNode );

	static void printValue( const D3DXVECTOR3& vec3 )
	{
		printf( "(%.2f, %.2f, %.2f)", vec3.x, vec3.y, vec3.z );
	}
	static void printValue( const TileRegion& tr )
	{
		printf( "(%d,%d)~(%d,%d)", tr.getStart().x, tr.getStart().y, tr.getEnd().x, tr.getEnd().y );
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
