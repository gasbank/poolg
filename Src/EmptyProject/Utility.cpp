#include "EmptyProjectPCH.h"
#include "Utility.h"
#include "ArnMesh.h"
#include "ArnMath.h"
#include "ArnConsts.h"

Utility::Utility(void)
{
}

Utility::~Utility(void)
{
}

// 주어진 축에 대고 벡터를 각도만큼 돌린다.
void Utility::rotateAboutAxis( ArnVec3* pvOut, ArnVec3* pvAxis, float rad )
{
	ArnMatrix rotMat;
	ArnVec4 vOutTmp;

	// 축을 Normalize한다.
	ArnVec3Normalize( pvAxis, pvAxis );

	// 회전 변환 행렬을 구한다.
	ArnMatrixRotationAxis( &rotMat, pvAxis, rad );
	const ArnMatrix rotMatConst( rotMat );

	// 회전 변환을 적용한다.
	ArnVec3Transform( &vOutTmp, pvOut, &rotMatConst );

	pvOut->x = vOutTmp.x;
	pvOut->y = vOutTmp.y;
	pvOut->z = vOutTmp.z;
}

float Utility::radBetweenVectors( const ArnVec3* v1, const ArnVec3* v2 )
{
	// A (dot product) B = abs(A) * abs(B) * cos(rad)

	float AdotB = ArnVec3Dot( v1, v2 );

	float absA = ArnVec3Length( v1 );
	float absB = ArnVec3Length( v2 );

	return acos( AdotB / ( absA * absB ) );
}

float MeshRayClosestIntersectDist( LPD3DXMESH mesh, const ArnVec3& rayStartPos, const ArnVec3& rayDir )
{
	HRESULT hr;
	bool hit;
	DWORD hitFaceIndex;
	float hitU, hitV;
	float hitDist;

	// Get intersection information
	V( ArnIntersectDx9( mesh, &rayStartPos, &rayDir, &hit,  &hitFaceIndex, &hitU, &hitV, &hitDist, 0, 0 ) );

	// If there is collision between ray and face
	if ( hit )
	{
		//printf( "Ray Testing Hit! Dist: %.2f\n", hitDist );
		return hitDist;
	}
	else
	{
		//printf( "Ray Testing Not Hit\n", hitDist );
		return FLOAT_POS_INF;
	}
}

// If a ray intersects a triangle within distMin, true is returned.
// Otherwise false is returned.
bool Utility::FullTraverseExhaustiveRayTesting( 
	ArnNode* node, const ArnVec3& rayStartPos, const ArnVec3& rayDir, float distMin )
{
	float dist = FullTraverseExhaustiveRayTesting( node, rayStartPos, rayDir, 1 );
	if ( dist < distMin )
		return true;
	else
		return false;
}

// return distance to the nth closest mesh
float Utility::FullTraverseExhaustiveRayTesting( 
	ArnNode* node, const ArnVec3& rayStartPos, const ArnVec3& rayDir, int nth )
{
	int count;
	float nthClosestDist;
	FullTraverseExhaustiveRayTesting( node, rayStartPos, rayDir, nth, &nthClosestDist, &count );
	return nthClosestDist;
}

// return the number of intersected meshes
int	Utility::FullTraverseExhaustiveRayTesting( 
	ArnNode* node, const ArnVec3& rayStartPos, const ArnVec3& rayDir )
{
	int count;
	float nthClosestDist;
	FullTraverseExhaustiveRayTesting( node, rayStartPos, rayDir, 1, &nthClosestDist, &count );
	return count;
}

void Utility::FullTraverseExhaustiveRayTesting( 
	ArnNode* node, 
	const ArnVec3& rayStartPos, 
	const ArnVec3& rayDir,
	int nth, 
	float* pNthClosestDist, 
	int* pIntersectedMeshCount
	)
{
	if ( node->getType() == NDT_RT_MESH )
	{
		ArnMesh* mesh = static_cast<ArnMesh*>( node );
		float dist = 0;
		if ( mesh->isCollide() )
		{	
			const ArnMatrix& meshXform = mesh->getFinalLocalXform();
			ArnVec3 relativeRayStartPos = ArnConsts::D3DXVEC3_ZERO;
			ArnVec3 relativeRayDir = ArnConsts::D3DXVEC3_ZERO;

			if ( mesh->getIpoName().length() == 0 )
			{
				relativeRayStartPos = rayStartPos - mesh->getLocalXform_Trans();
				dist = MeshRayClosestIntersectDist( mesh->getD3DXMesh(), relativeRayStartPos, rayDir );
			}
			else
			{
				ArnMatrix meshXformInv;
				ArnVec3 vScale, vTrans;
				ArnQuat qRot;
				ArnMatrixInverse( &meshXformInv, 0, &meshXform );
				ArnMatrixDecompose( &vScale, &qRot, &vTrans, &meshXformInv );
				ArnMatrix mRot;
				ArnMatrixRotationQuaternion( &mRot, &qRot );
				ArnVec3TransformCoord( &relativeRayStartPos, &rayStartPos, &meshXformInv );
				ArnVec3TransformCoord( &relativeRayDir, &rayDir, &mRot );
				dist = MeshRayClosestIntersectDist( mesh->getD3DXMesh(), relativeRayStartPos, relativeRayDir );
			}
		}
		else
		{
			dist = FLOAT_POS_INF;
		}
		

		if ( dist == FLOAT_POS_INF )
			*pIntersectedMeshCount = 0;
		else
			*pIntersectedMeshCount = 1;
		*pNthClosestDist = dist;
	}
	else
	{
		UINT i;
		const UINT childCount = node->getNodeCount();
		std::vector<float> distVector;
		distVector.push_back( FLOAT_POS_INF );
		for ( i = 0; i < childCount; ++i )
		{
			float newDist = FullTraverseExhaustiveRayTesting( 
				node->getNodeAt( i ), rayStartPos, rayDir, 1 );
			distVector.push_back( newDist );
		}
		sort( distVector.begin(), distVector.end() );

		*pIntersectedMeshCount = distVector.size() - 1;
		*pNthClosestDist = distVector[--nth];
	}
}

//int Utility::intersectionsBetweenTwoPosition( 
//	const ArnVec3* v1, const ArnVec3* v2, ArnNode* arnNode )
//{
//	ArnVec3 vStartPos( *v1 );
//	ArnVec3 vRayDir = *v2 - *v1;
//	D3DXVec3Normalize( &vRayDir, &vRayDir );
//	return FullTraverseExhaustiveRayTesting( arnNode, vStartPos, vRayDir );
//}