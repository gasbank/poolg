#include "EmptyProjectPCH.h"
#include "EpCamera.h"

EpCamera::EpCamera(void)
{
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

//void EpCamera::SetViewParams3( D3DXVECTOR3* vecEye, D3DXVECTOR3* vecAt, D3DXVECTOR3* vecUp )
//{
//	D3DXVECTOR3 eye(-10.0f,-10.0f,-10.0f);
//	D3DXVECTOR3 at(0,0,0);
//	D3DXVECTOR3 up(0,0,-1.0f);
//
//	D3DXMatrixLookAtLH(&m_mView, vecEye, vecAt, vecUp);
//
//	//*(D3DXVECTOR3*)&m_mCameraWorld._21 = *vecUp;
//}