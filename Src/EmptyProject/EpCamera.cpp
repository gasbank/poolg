#include "EmptyProjectPCH.h"
#include "EpCamera.h"


EpCamera::EpCamera(void)
{
	m_fSmoothCameraTimer = 99999.0f;
	m_fSmoothCameraDuration = 1.0f;
	m_bViewParamsDirty = false;
	m_bCamManualMovement = false;
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
	}

	if ( m_bViewParamsDirty && !m_bCamManualMovement )
	{
		setViewParamsWithUp( &m_vEye, &m_vLookAt, &m_vUp );
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

	const ARN_NDD_CAMERA_CHUNK& arnCamData = arnCam->getCameraData();

	// Extract information from localXfrom
	const D3DXMATRIX& arnCamLocalXfrom = m_pArnCam->getFinalLocalXform();

	m_vUp.x = arnCamLocalXfrom._21;
	m_vUp.y = arnCamLocalXfrom._22;
	m_vUp.z = arnCamLocalXfrom._23;

	m_vLookAt.x = arnCamLocalXfrom._31;
	m_vLookAt.y = arnCamLocalXfrom._32;
	m_vLookAt.z = arnCamLocalXfrom._33;

	m_vEye.x = arnCamLocalXfrom._41;
	m_vEye.y = arnCamLocalXfrom._42;
	m_vEye.z = arnCamLocalXfrom._43;

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
	// ������ �ð� ���� ���� ��ġ�κ��� ���� ��ġ���� linear interpolation �Ͽ� ���� ī�޶� ��ġ��
	// ���Ѵ�.
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
		// ���������� ���� ��ġ�� ��Ȯ�� ī�޶� ���´�.
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
		m_vDesLookAt.z = m_vPos->z;

		m_vDesUp.x = 0.0f;
		m_vDesUp.y = 1.0f;
		m_vDesUp.z = 0.0f;
	}
	else
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
	}

	// And move to the destination gradually
	updateSmoothCamera( fElapsedTime );

	// After moving to destination, always follow object.
	m_bViewParamsDirty = true;
}