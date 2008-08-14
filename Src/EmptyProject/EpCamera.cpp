#include "EmptyProjectPCH.h"
#include "EpCamera.h"


EpCamera::EpCamera(void)
{
	m_fSmoothCameraTimer = 99999.0f;
}

void EpCamera::frameMove( FLOAT fElapsedTime )
{
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
	}

	CModelViewerCamera::FrameMove( fElapsedTime );
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
	// [  Right ]
	// [   Up   ]
	// [  Look  ]
	// [Position]
	//
	// Transpose of this is view transform matrix
	//

	const ARN_NDD_CAMERA_CHUNK& arnCamData = arnCam->getCameraData();

	// Extract information from localXfrom
	const D3DXMATRIX& arnCamLocalXfrom = m_pArnCam->getFinalLocalXform();
	D3DXVECTOR3 vUp;
	D3DXVECTOR3 vEye;
	D3DXVECTOR3 vLookAt;

	vUp.x = arnCamLocalXfrom._21;
	vUp.y = arnCamLocalXfrom._22;
	vUp.z = arnCamLocalXfrom._23;

	vLookAt.x = arnCamLocalXfrom._31;
	vLookAt.y = arnCamLocalXfrom._32;
	vLookAt.z = arnCamLocalXfrom._33;

	vEye.x = arnCamLocalXfrom._41;
	vEye.y = arnCamLocalXfrom._42;
	vEye.z = arnCamLocalXfrom._43;

	setViewParamsWithUp( &vEye, &vLookAt, vUp );
}

void EpCamera::setDesViewParams( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookAtPt, const D3DXVECTOR3& vUp )
{
	m_vDesEye = *pvEyePt;
	m_vDesLookAt = *pvLookAtPt;
	m_vDesUp = vUp;
}

void EpCamera::updateSmoothCamera( float fElapsedTime )
{
	// 지정된 시간 동안 이전 위치로부터 목적 위치까지 linear interpolation 하여 현재 카메라 위치를
	// 구한다.
	if ( m_fSmoothCameraTimer <= m_fSmoothCameraDuration )
	{
		float s = m_fSmoothCameraTimer / m_fSmoothCameraDuration;

		D3DXVec3Lerp( &m_vEye, &m_vPrevEye, &m_vDesEye, s );
		D3DXVec3Lerp( &m_vLookAt, &m_vPrevLookAt, &m_vDesLookAt, s );
		D3DXVec3Lerp( &m_vUp, &m_vPrevUp, &m_vDesUp, s );

		m_fSmoothCameraTimer += fElapsedTime;
	} 
	else 
	{
		m_vEye = m_vDesEye;
		m_vLookAt = m_vDesLookAt;
		m_vUp	= m_vDesUp;
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
		m_vPrevEye = m_vEye;
		m_vPrevLookAt = m_vPrevLookAt;
		m_vPrevUp = m_vUp;
		m_fSmoothCameraTimer = 0.0f;
		break;
	}
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