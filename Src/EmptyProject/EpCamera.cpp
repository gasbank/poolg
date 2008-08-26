#include "EmptyProjectPCH.h"
#include "EpCamera.h"
#include "Utility.h"

#include "WorldManager.h"
#include "World.h"
#include "ArnNode.h"


EpCamera::EpCamera(void)
{
	m_fSmoothCameraTimer = 99999.0f;
	m_fSmoothCameraDuration = 1.0f;
	m_bViewParamsDirty = false;
	m_bCamManualMovement = false;

	m_bShake = false;
	m_nextShakeTime = 0;

	m_vEyeShake = m_vUpShake = m_vLookAtShake = DX_CONSTS::D3DXVEC3_ZERO;
}

void EpCamera::frameMove( FLOAT fElapsedTime )
{
	CModelViewerCamera::FrameMove( fElapsedTime );

	switch ( m_runningCamera )
	{
	case CAMERA_NORMAL:
		// 위치가 일정하므로 Update해줄 것이 없다.
		break;
	case CAMERA_SMOOTH:
		updateSmoothCamera( fElapsedTime );
		break;
	case CAMERA_EXTERNAL:
		updateExternalCamera( m_pArnCam );
		break;
	case CAMERA_SMOOTH_ATTACH:
		updateSmoothAttachCamera( fElapsedTime );
		break;
	case CAMERA_ATTACH:
		updateAttachCamera();
		break;
	}

	//printf("Eye, At %f %f %f, %f %f %f \n", m_vEye.x, m_vEye.y, m_vEye.z, m_vLookAt.x, m_vLookAt.y, m_vLookAt.z );

	processShake( fElapsedTime );
	
	if ( ( m_bViewParamsDirty && !m_bCamManualMovement ) || m_bShake )
	{
		if ( m_bShake )
		{
			D3DXVECTOR3 eye, lookAt, up;
			eye		= m_vEye + m_vEyeShake;
			lookAt	= m_vLookAt + m_vLookAtShake;
			up		= m_vUp + m_vUpShake;
			setViewParamsWithUp( &eye, &lookAt, &up );
		}
		else
		{
			setViewParamsWithUp( &m_vEye, &m_vLookAt, &m_vUp );
		}
		
		m_bViewParamsDirty = false;
	}
}

D3DUtil_CameraKeys EpCamera::MapKey( UINT nKey )
{
	// This could be upgraded to a method that's user-definable but for 
	// simplicity, we'll use a hardcoded mapping.
	switch( nKey )
	{
	case VK_CONTROL:
		return CAM_CONTROLDOWN;
	case VK_LEFT:
		return CAM_STRAFE_LEFT;
	case VK_RIGHT:
		return CAM_STRAFE_RIGHT;
	case VK_UP:
		return CAM_MOVE_FORWARD;
	case VK_DOWN:
		return CAM_MOVE_BACKWARD;
	case VK_PRIOR:
		return CAM_MOVE_UP;        // pgup
	case VK_NEXT:
		return CAM_MOVE_DOWN;      // pgdn

	case VK_HOME:
		return CAM_RESET;
	}

	return CAM_UNKNOWN;
}

void EpCamera::setViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp )
{
	if( NULL == pvEyePt || NULL == pvLookatPt )
		return;

	m_vDefaultEye = m_vEye = *pvEyePt;
	m_vDefaultLookAt = m_vLookAt = *pvLookatPt;
	m_vUp = vUp;

	// Calc the view matrix
	D3DXMatrixLookAtLH( &m_mView, pvEyePt, pvLookatPt, &vUp );

	D3DXMATRIX mInvView;
	D3DXMatrixInverse( &mInvView, NULL, &m_mView );

	// The axis basis vectors and camera position are stored inside the 
	// position matrix in the 4 rows of the camera's world matrix.
	// To figure out the yaw/pitch of the camera, we just need the Z basis vector

	D3DXVECTOR3* pZBasis = ( D3DXVECTOR3* )&mInvView._31;

	m_fCameraYawAngle = atan2f( pZBasis->x, pZBasis->z );
	float fLen = sqrtf( pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x );
	m_fCameraPitchAngle = -atan2f( pZBasis->y, fLen );


	// Propogate changes to the member arcball
	D3DXQUATERNION quat;
	D3DXMATRIXA16 mRotation;
	D3DXMatrixLookAtLH( &mRotation, pvEyePt, pvLookatPt, &vUp );
	D3DXQuaternionRotationMatrix( &quat, &mRotation );
	m_ViewArcBall.SetQuatNow( quat );

	// Set the radius according to the distance
	D3DXVECTOR3 vEyeToPoint;
	D3DXVec3Subtract( &vEyeToPoint, pvLookatPt, pvEyePt );
	SetRadius( D3DXVec3Length( &vEyeToPoint ) );

	// View information changed. FrameMove should be called.
	m_bDragSinceLastUpdate = true;
}

void EpCamera::setViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, D3DXVECTOR3* pvUp )
{
	setViewParamsWithUp( pvEyePt, pvLookatPt, *pvUp );

}
D3DXVECTOR3* EpCamera::GetUpPt()
{
	return &m_vUp;
}

// arnCam로부터 정보를 얻어서 카메라의 view parameter를 지정한다.
// arnCam이 변할 수 있으므로 프레임마다 호출해아 한다.
void EpCamera::updateExternalCamera( ArnCamera* arnCam )
{
	//
	// Content of local transform ( 4 * 4)
	//
	// [  Right ]
	// [   Up   ]
	// [  Look  ]
	// [Position]
	//
	// Transpose of this is view transform matrix
	//
	assert( arnCam );

	const ARN_NDD_CAMERA_CHUNK& arnCamData = arnCam->getCameraData();

	// Extract information from localXfrom
	D3DXMATRIX arnCamLocalXfrom = m_pArnCam->getFinalLocalXform();

	// Calculate up and lookAt vectors based on local xform mat
	D3DXVECTOR3 pos = *(D3DXVECTOR3*)&arnCamLocalXfrom._41;		// Camera position stored in 4th row of mat
	D3DXVECTOR3 up = DX_CONSTS::D3DXVEC3_Y;						// Up vector basis (+Y axis)
	D3DXVECTOR3 look = DX_CONSTS::D3DXVEC3_Z;					// LookAt vector basis (+Z axis)
	D3DXVec3TransformCoord( &look, &look, &arnCamLocalXfrom );	// Transform LookAt vector
	
	// Clear translation part of camera xform mat since Up vector is relative to
	// camera position, not absolute.
	arnCamLocalXfrom._41 = arnCamLocalXfrom._42 = arnCamLocalXfrom._43 = 0;
	D3DXVec3TransformCoord( &up, &up, &arnCamLocalXfrom );		// Transform Up vector

	m_vUp = up;
	m_vLookAt = look;
	m_vEye = pos;

	m_bViewParamsDirty = true;
}

void EpCamera::setDesViewParams( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookAtPt, D3DXVECTOR3* vUp )
{
	m_vDesEye = *pvEyePt;
	m_vDesLookAt = *pvLookAtPt;
	m_vDesUp = *vUp;
}

void EpCamera::updateSmoothCamera( float fElapsedTime )
{
	// 지정된 시간 동안 이전 위치로부터 목적 위치까지 linear interpolation 하여 현재 카메라 위치를
	// 구한다.
	if ( m_fSmoothCameraTimer < m_fSmoothCameraDuration )
	{
		float s = m_fSmoothCameraTimer / m_fSmoothCameraDuration;

		D3DXVec3Lerp( &m_vEye, &m_vPrevEye, &m_vDesEye, s );

		D3DXVec3Lerp( &m_vLookAt, &m_vPrevLookAt, &m_vDesLookAt, s );
		D3DXVec3Lerp( &m_vUp, &m_vPrevUp, &m_vDesUp, s );

		m_fSmoothCameraTimer += fElapsedTime;

		m_bViewParamsDirty = true;
	} 
	else if ( m_bUpdateContinue )
	{
		// 마지막으로 최종 위치에 정확히 카메라를 놓는다.
		m_vEye = m_vDesEye;
		m_vLookAt = m_vDesLookAt;
		m_vUp	= m_vDesUp;

		m_bUpdateContinue = false;
		m_bViewParamsDirty = true;
	}
}

void EpCamera::begin( RunningCamera rc )
{
	m_runningCamera = rc;

	// CAMERA_SMOOTH는 카메라가 이전 위치에서 목적 위치까지 서서히 움직이는 카메라이다.
	// 따라서 이전 위치를 미리 저장해 두고, timer도 초기화한다.
	switch ( rc )
	{
	case CAMERA_SMOOTH:
	case CAMERA_SMOOTH_ATTACH:
		m_vPrevEye = m_vEye;
		m_vPrevLookAt = m_vLookAt;
		m_vPrevUp = m_vUp;
		m_fSmoothCameraTimer = 0.0f;
		break;
	}

	m_bUpdateContinue = true;
}

// 쓸모 없는 함수가 된듯 하나 아까워서 남겨둠.
void EpCamera::lerpViewParams(
	D3DXVECTOR3* pvEyeOut, D3DXVECTOR3* pvLookAtOut, D3DXVECTOR3* pvUpOut, 
	D3DXVECTOR3* pvEye1, D3DXVECTOR3* pvLookAt1, D3DXVECTOR3* pvUp1, 
	D3DXVECTOR3* pvEye2, D3DXVECTOR3* pvLookAt2, D3DXVECTOR3* pvUp2, 
	float s )
{
	D3DXVec3Lerp( pvEyeOut, pvEye1, pvEye2, s );
	D3DXVec3Lerp( pvLookAtOut, pvLookAt1, pvLookAt2, s );
	D3DXVec3Lerp( pvUpOut, pvUp1, pvUp2, s );
}

LRESULT EpCamera::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_KEYDOWN )
		if ( wParam == VK_F6 )
			m_bCamManualMovement = !m_bCamManualMovement;

	return CModelViewerCamera::HandleMessages( hWnd, uMsg, wParam, lParam );
}

void EpCamera::updateSmoothAttachCamera( float fElapsedTime )
{
	// 기존의 Smooth moving camera를 활용한다.
	// At first, attach destination of camera to object's position.
	// So camera gradually attached.
	// If attaching is finished, just float over the object and follow object.
	if ( m_bUpdateContinue )
	{
		m_vDesEye.x = m_vPos->x;
		m_vDesEye.y = m_vPos->y;
		m_vDesEye.z = m_vPos->z - 30.0f;

		m_vDesLookAt.x = m_vPos->x;
		m_vDesLookAt.y = m_vPos->y;
		m_vDesLookAt.z = m_vPos->z - 1.0f;

		m_vDesUp.x = 0.0f;
		m_vDesUp.y = 1.0f;
		m_vDesUp.z = 0.0f;

		pulledEye( &m_vDesEye, &m_vDesLookAt, &m_vDesEye, 1 );
	}
	else
	{
		m_vEye.x = m_vPos->x;
		m_vEye.y = m_vPos->y;
		m_vEye.z = m_vPos->z - 30.0f;

		m_vLookAt.x = m_vPos->x;
		m_vLookAt.y = m_vPos->y;
		m_vLookAt.z = m_vPos->z - 1.0f;

		m_vUp.x = 0.0f;
		m_vUp.y = 1.0f;
		m_vUp.z = 0.0f;

		pulledEye( &m_vEye, &m_vLookAt, &m_vEye, 1 );
	}

	// And move to the destination gradually
	updateSmoothCamera( fElapsedTime );

	// After moving to destination, always follow object.
	m_bViewParamsDirty = true;
}

void EpCamera::beginShoulderLookCamera( const D3DXVECTOR3* pvMePos, const D3DXVECTOR3* pvOppPos )
{
	D3DXVECTOR3 vEnemyPos = *pvOppPos;
	D3DXVECTOR3 vHeroPos = *pvMePos;

	// 전투가 일어나는 위치를 구한다. 적의 위치와 주인공 위치의 중간임.
	D3DXVECTOR3 vBattlePos;
	vBattlePos.x = (vEnemyPos.x + vHeroPos.x) / 2.0f;
	vBattlePos.y = (vEnemyPos.y + vHeroPos.y) / 2.0f;
	vBattlePos.z = (vEnemyPos.z + vHeroPos.z) / 2.0f;

	// 최종 카메라 상태.
	D3DXVECTOR3 vDesEye( 0.0f, 0.0f, -10.0f );
	D3DXVECTOR3 vDesLookAt( vBattlePos.x, vBattlePos.y, vBattlePos.z - 2.0f );
	D3DXVECTOR3 vDesUp( 0.0f, 0.0f, -1.0f );

	// 주인공으로부터 적으로 이어지는 축을 구한다.
	D3DXVECTOR3 vBattleAxis;
	vBattleAxis.x = (vEnemyPos.x - vHeroPos.x);
	vBattleAxis.y = (vEnemyPos.y - vHeroPos.y);
	vBattleAxis.z = (vEnemyPos.z - vHeroPos.z);

	// z 축에 대고 vBattleAxis를 -45도 돌린다.
	D3DXVECTOR3 zAxis( 0.0f, 0.0f, 1.0f );
	Utility::rotateAboutAxis( &vBattleAxis, &zAxis, D3DXToRadian( -30.0f ) );

	// vDesEye를 vBattleAxis에 대고 -60도 만큼 돌린다.
	Utility::rotateAboutAxis( &vDesEye, &vBattleAxis, D3DXToRadian( -45.0f ) );

	// vDesEye를 주인공 위로 옮긴다.
	vDesEye.x += vBattlePos.x;
	vDesEye.y += vBattlePos.y;

	vDesLookAt.z -= 1.0f;

	// 장애물에 가리지 않도록 한다.
	pulledEye( &vDesEye, &vDesLookAt, &vDesEye, 1 );

	// 자, 이제 카메라의 목적 위치를 구하였다. 이동시켜보자.
	setDesViewParams( &vDesEye, &vDesLookAt, &vDesUp );
	setSmoothCameraDuration( 1.0f );
	begin( CAMERA_SMOOTH );
}

void EpCamera::updateAttachCamera()
{
	m_vEye.x = m_vPos->x;
	m_vEye.y = m_vPos->y;
	m_vEye.z = m_vPos->z - 30.0f;
	m_vLookAt.x = m_vPos->x;
	m_vLookAt.y = m_vPos->y;
	m_vLookAt.z = m_vPos->z;
	m_vUp.x = 0.0f;
	m_vUp.y = 1.0f;
	m_vUp.z = 0.0f;

	m_bViewParamsDirty = true;
}

void EpCamera::pulledEye( D3DXVECTOR3* vPulledEye, D3DXVECTOR3* vLookAt, D3DXVECTOR3* vEye, int nth )
{
	ArnNode* arnNode = GetWorldManager().getCurWorld()->getArnSceneGraphPt()->getSceneRoot();
	D3DXVECTOR3 vRayDir = *vEye - *vLookAt;
	D3DXVECTOR3 vNormRayDir;
	D3DXVec3Normalize( &vNormRayDir, &vRayDir );
	float camDist = D3DXVec3Length( &vRayDir );
	float obsDist = Utility::FullTraverseExhaustiveRayTesting( arnNode, *vLookAt, vNormRayDir, nth );

	if ( camDist <= obsDist )
		*vPulledEye = *vEye;
	else
	{
		vNormRayDir *= obsDist;
		*vPulledEye = *vLookAt + vNormRayDir;
	}

	//printf( "camDist, obsDist %f %f \n", camDist, obsDist );
}

void EpCamera::processShake( float fElapsedTime )
{
	if ( m_bShake )
	{
		const float shakeIntervalMax	= 0.075f;

		const float eyeShakeAmount		= 0.5f;
		const float lookAtShakeAmount	= 0.2f;
		const float upShakeAmount		= 0.075f;

		if ( m_nextShakeTime <= 0 )
		{
			m_vEyeShake = D3DXVECTOR3(
				( (float)rand()/RAND_MAX - 0.5f ) * eyeShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * eyeShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * eyeShakeAmount );
			m_vLookAtShake = D3DXVECTOR3(
				( (float)rand()/RAND_MAX - 0.5f ) * lookAtShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * lookAtShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * lookAtShakeAmount );
			m_vUpShake = D3DXVECTOR3(
				( (float)rand()/RAND_MAX - 0.5f ) * upShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * upShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * upShakeAmount );

			m_bViewParamsDirty = true;

			m_nextShakeTime = (float)rand()/RAND_MAX * shakeIntervalMax;
		}
		else
		{
			m_nextShakeTime -= fElapsedTime;
		}
	}
}