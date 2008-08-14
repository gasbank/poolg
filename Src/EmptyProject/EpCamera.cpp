#include "EmptyProjectPCH.h"
#include "EpCamera.h"


EpCamera::EpCamera(void)
{
}

void EpCamera::FrameMove( FLOAT fElapsedTime )
{
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

void EpCamera::SetViewParamsWithUp( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt, const D3DXVECTOR3& vUp )
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

void EpCamera::beginExternalCamera( ArnCamera* arnCam )
{
	m_pArnCam = arnCam;
	m_runningCamera = CAMERA_EXTERNAL;
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

	SetViewParamsWithUp( &vEye, &vLookAt, vUp );
}
