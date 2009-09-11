#include "EmptyProjectPCH.h"
#include "EpCamera.h"
#include "Utility.h"

#include "WorldManager.h"
#include "World.h"
#include "ArnNode.h"
#include "ShaderWrapper.h"

extern PostRadialBlurShader*			g_postRadialBlurShader;

EpCamera::EpCamera(void)
{
	m_fSmoothCameraTimer = 99999.0f;
	m_fSmoothCameraDuration = 1.0f;
	m_bViewParamsDirty = false;
	m_bCamManualMovement = false;

	m_bShake = false;
	m_nextShakeTime = 0;
	m_minPullDist = 5.0f;

	m_vEyeShake = m_vUpShake = m_vLookAtShake = ArnConsts::ARNVEC3_ZERO;
}

void EpCamera::frameMove( FLOAT fElapsedTime )
{
	CModelViewerCamera::FrameMove( fElapsedTime );

	switch ( m_runningCamera )
	{
	case CAMERA_NORMAL:
		// ��ġ�� �����ϹǷ� Update���� ���� ����.
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
		ArnVec3 eye, lookAt, up;
		if ( m_bShake )
		{
			
			eye		= m_vEyeShake + m_vEye;
			lookAt	= m_vLookAtShake + m_vLookAt;
			up		= m_vUpShake + m_vUp;
			setViewParamsWithUp( &eye, &lookAt, &up );
		}
		else
		{
			eye		= m_vEye;
			lookAt	= m_vLookAt;
			up		= m_vUp;
			setViewParamsWithUp( &eye, &lookAt, &up );
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

void EpCamera::setViewParamsWithUp( ArnVec3* pvEyePt, ArnVec3* pvLookatPt, const ArnVec3& vUp )
{
	if( NULL == pvEyePt || NULL == pvLookatPt )
		return;

	m_vEye.x = pvEyePt->x;
	m_vEye.y = pvEyePt->y;
	m_vEye.z = pvEyePt->z;

	m_vDefaultEye.x = pvEyePt->x;
	m_vDefaultEye.y = pvEyePt->y;
	m_vDefaultEye.z = pvEyePt->z;

	m_vLookAt.x = pvLookatPt->x;
	m_vLookAt.y = pvLookatPt->y;
	m_vLookAt.z = pvLookatPt->z;

	m_vDefaultLookAt.x = pvLookatPt->x;
	m_vDefaultLookAt.y = pvLookatPt->y;
	m_vDefaultLookAt.z = pvLookatPt->z;
	m_vUp = vUp;

	// TODO: Should be verified...

	// Calc the view matrix
	ArnMatrix view;
	ArnMatrixLookAtLH( &view, pvEyePt, pvLookatPt, &vUp );
	view = view.transpose();
	memcpy(&m_mView, &view, sizeof(float)*4*4);

	ArnMatrix mInvView;
	ArnMatrixInverse( &mInvView, NULL, reinterpret_cast<ArnMatrix*>(&m_mView) );

	// The axis basis vectors and camera position are stored inside the 
	// position matrix in the 4 rows of the camera's world matrix.
	// To figure out the yaw/pitch of the camera, we just need the Z basis vector

	ArnVec3* pZBasis = ( ArnVec3* )&mInvView.m[3][1];

	m_fCameraYawAngle = atan2f( pZBasis->x, pZBasis->z );
	float fLen = sqrtf( pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x );
	m_fCameraPitchAngle = -atan2f( pZBasis->y, fLen );


	// Propogate changes to the member arcball
	ArnQuat quat;
	ArnMatrix mRotation;
	ArnMatrixLookAtLH( &mRotation, pvEyePt, pvLookatPt, &vUp );
	ArnQuaternionRotationMatrix( &quat, &mRotation );
	D3DXQUATERNION d3dxQ(quat.x, quat.y, quat.z, quat.w);
	m_ViewArcBall.SetQuatNow( d3dxQ );

	// Set the radius according to the distance
	ArnVec3 vEyeToPoint;
	vEyeToPoint = *pvLookatPt - *pvEyePt;
	SetRadius( ArnVec3Length( &vEyeToPoint ) );

	// View information changed. FrameMove should be called.
	m_bDragSinceLastUpdate = true;
}

void EpCamera::setViewParamsWithUp( ArnVec3* pvEyePt, ArnVec3* pvLookatPt, ArnVec3* pvUp )
{
	setViewParamsWithUp( pvEyePt, pvLookatPt, *pvUp );

}
ArnVec3* EpCamera::GetUpPt()
{
	return &m_vUp;
}

// arnCam�κ��� ������ �� ī�޶��� view parameter�� �����Ѵ�.
// arnCam�� ���� �� �����Ƿ� �����Ӹ��� ȣ���ؾ� �Ѵ�.
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
	ArnMatrix arnCamLocalXfrom(m_pArnCam->computeWorldXform());

	// Calculate up and lookAt vectors based on local xform mat
	ArnVec3 pos = *(ArnVec3*)&arnCamLocalXfrom.m[3][0];		// Camera position stored in 4th row of mat
	ArnVec3 up = ArnConsts::ARNVEC3_Y;						// Up vector basis (+Y axis)
	ArnVec3 look = ArnConsts::ARNVEC3_Z;					// LookAt vector basis (+Z axis)
	ArnVec3TransformCoord( &look, &look, &arnCamLocalXfrom );	// Transform LookAt vector
	
	// Clear translation part of camera xform mat since Up vector is relative to
	// camera position, not absolute.
	arnCamLocalXfrom.m[3][0] = arnCamLocalXfrom.m[3][1] = arnCamLocalXfrom.m[3][2] = 0;
	ArnVec3TransformCoord( &up, &up, &arnCamLocalXfrom );		// Transform Up vector

	m_vUp = up;
	ArnVec3Assign(m_vLookAt, look);
	ArnVec3Assign(m_vEye, pos);

	m_bViewParamsDirty = true;
}

void EpCamera::setDesViewParams( ArnVec3* pvEyePt, ArnVec3* pvLookAtPt, ArnVec3* vUp )
{
	m_vDesEye = *pvEyePt;
	m_vDesLookAt = *pvLookAtPt;
	m_vDesUp = *vUp;
}

void EpCamera::updateSmoothCamera( float fElapsedTime )
{
	// ������ �ð� ���� ���� ��ġ�κ��� ���� ��ġ���� linear interpolation �Ͽ� ���� ī�޶� ��ġ��
	// ���Ѵ�.
	if ( m_fSmoothCameraTimer < m_fSmoothCameraDuration )
	{
		float s = m_fSmoothCameraTimer / m_fSmoothCameraDuration;

		ArnVec3Lerp( &m_vEye, &m_vPrevEye, &m_vDesEye, s );

		ArnVec3Lerp( &m_vLookAt, &m_vPrevLookAt, &m_vDesLookAt, s );
		ArnVec3Lerp( &m_vUp, &m_vPrevUp, &m_vDesUp, s );

		m_fSmoothCameraTimer += fElapsedTime;

		m_bViewParamsDirty = true;

		const float blurWidth = 0.1f;
		if ( s < 0.5f )
			g_postRadialBlurShader->setBlurWidth( (s * 2) * blurWidth );
		else
			g_postRadialBlurShader->setBlurWidth( (2.0f + (-s * 2)) * blurWidth );
	} 
	else if ( m_bUpdateContinue )
	{
		// ���������� ���� ��ġ�� ��Ȯ�� ī�޶� ���´�.
		ArnVec3Assign(m_vEye, m_vDesEye);
		ArnVec3Assign(m_vLookAt, m_vDesLookAt);
		m_vUp	= m_vDesUp;

		m_bUpdateContinue = false;
		m_bViewParamsDirty = true;

		g_postRadialBlurShader->setBlurWidth( 0 );
	}
}

void EpCamera::begin( RunningCamera rc )
{
	m_runningCamera = rc;

	// CAMERA_SMOOTH�� ī�޶� ���� ��ġ���� ���� ��ġ���� ������ �����̴� ī�޶��̴�.
	// ���� ���� ��ġ�� �̸� ������ �ΰ�, timer�� �ʱ�ȭ�Ѵ�.
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

// ���� ���� �Լ��� �ȵ� �ϳ� �Ʊ���� ���ܵ�.
void EpCamera::lerpViewParams(
	ArnVec3* pvEyeOut, ArnVec3* pvLookAtOut, ArnVec3* pvUpOut, 
	ArnVec3* pvEye1, ArnVec3* pvLookAt1, ArnVec3* pvUp1, 
	ArnVec3* pvEye2, ArnVec3* pvLookAt2, ArnVec3* pvUp2, 
	float s )
{
	ArnVec3Lerp( pvEyeOut, pvEye1, pvEye2, s );
	ArnVec3Lerp( pvLookAtOut, pvLookAt1, pvLookAt2, s );
	ArnVec3Lerp( pvUpOut, pvUp1, pvUp2, s );
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
	// ������ Smooth moving camera�� Ȱ���Ѵ�.
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

		pulledEye( reinterpret_cast<ArnVec3*>(&m_vEye), reinterpret_cast<ArnVec3*>(&m_vLookAt), reinterpret_cast<ArnVec3*>(&m_vEye), 1 );
	}

	// And move to the destination gradually
	updateSmoothCamera( fElapsedTime );

	// After moving to destination, always follow object.
	m_bViewParamsDirty = true;
}

void EpCamera::beginShoulderLookCamera( const ArnVec3* pvMePos, const ArnVec3* pvOppPos )
{
	ArnVec3 vEnemyPos = *pvOppPos;
	ArnVec3 vHeroPos = *pvMePos;

	// ������ �Ͼ�� ��ġ�� ���Ѵ�. ���� ��ġ�� ���ΰ� ��ġ�� �߰���.
	ArnVec3 vBattlePos;
	vBattlePos.x = (vEnemyPos.x + vHeroPos.x) / 2.0f;
	vBattlePos.y = (vEnemyPos.y + vHeroPos.y) / 2.0f;
	vBattlePos.z = (vEnemyPos.z + vHeroPos.z) / 2.0f;

	// ���� ī�޶� ����.
	ArnVec3 vDesEye( 0.0f, 0.0f, -15.0f );
	ArnVec3 vDesLookAt( vBattlePos.x, vBattlePos.y, vBattlePos.z - 2.0f );
	ArnVec3 vDesUp( 0.0f, 0.0f, -1.0f );

	// ���ΰ����κ��� ������ �̾����� ���� ���Ѵ�.
	ArnVec3 vBattleAxis;
	vBattleAxis.x = (vEnemyPos.x - vHeroPos.x);
	vBattleAxis.y = (vEnemyPos.y - vHeroPos.y);
	vBattleAxis.z = (vEnemyPos.z - vHeroPos.z);

	// z �࿡ ��� vBattleAxis�� -45�� ������.
	ArnVec3 zAxis( 0.0f, 0.0f, 1.0f );
	Utility::rotateAboutAxis( &vBattleAxis, &zAxis, D3DXToRadian( -30.0f ) );

	// vDesEye�� vBattleAxis�� ��� -60�� ��ŭ ������.
	Utility::rotateAboutAxis( &vDesEye, &vBattleAxis, D3DXToRadian( -45.0f ) );

	// vDesEye�� ���ΰ� ���� �ű��.
	vDesEye.x += vBattlePos.x;
	vDesEye.y += vBattlePos.y;

	vDesLookAt.z -= 1.0f;

	// ��ֹ��� ������ �ʵ��� �Ѵ�.
	pulledEye( &vDesEye, &vDesLookAt, &vDesEye, 1 );

	// ��, ���� ī�޶��� ���� ��ġ�� ���Ͽ���. �̵����Ѻ���.
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

void EpCamera::pulledEye( ArnVec3* vPulledEye, ArnVec3* vLookAt, ArnVec3* vEye, int nth )
{
	ArnNode* arnNode = GetWorldManager().getCurWorld()->getArnSceneGraphPt()->getSceneRoot();
	ArnVec3 vRayDir = *vEye - *vLookAt;
	ArnVec3 vNormRayDir;
	ArnVec3Normalize( &vNormRayDir, &vRayDir );
	float camDist = ArnVec3Length( &vRayDir );
	float obsDist = Utility::FullTraverseExhaustiveRayTesting( arnNode, *vLookAt, vNormRayDir, nth );

	if ( camDist <= obsDist )
		*vPulledEye = *vEye;
	else
	{
		if ( obsDist <= m_minPullDist )
			pulledEye( vPulledEye, vLookAt, vEye, ++nth );
		else
		{
			vNormRayDir *= obsDist;
			*vPulledEye = *vLookAt + vNormRayDir;
		}
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
			m_vEyeShake = ArnVec3(
				( (float)rand()/RAND_MAX - 0.5f ) * eyeShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * eyeShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * eyeShakeAmount );
			m_vLookAtShake = ArnVec3(
				( (float)rand()/RAND_MAX - 0.5f ) * lookAtShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * lookAtShakeAmount,
				( (float)rand()/RAND_MAX - 0.5f ) * lookAtShakeAmount );
			m_vUpShake = ArnVec3(
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