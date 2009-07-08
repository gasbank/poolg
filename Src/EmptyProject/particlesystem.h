
//-----------------------------------------------------------------------------
//           Name: particlesystem.h
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: Header file for the CParticleSystem Class
//-----------------------------------------------------------------------------

#ifndef CPARTICLESYSTEM_H_INCLUDED
#define CPARTICLESYSTEM_H_INCLUDED

//-----------------------------------------------------------------------------
// SYMBOLIC CONSTANTS
//-----------------------------------------------------------------------------

// Classify Point
const int CP_FRONT   = 0;
const int CP_BACK    = 1;
const int CP_ONPLANE = 2;

// Collision Results
const int CR_BOUNCE  = 0;
const int CR_STICK   = 1;
const int CR_RECYCLE = 2;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

struct Plane
{
    ArnVec3 m_vNormal;           // The plane's normal
    ArnVec3 m_vPoint;            // A coplanar point within the plane
    float       m_fBounceFactor;     // Coefficient of restitution (or how bouncy the plane is)
    int         m_nCollisionResult;  // What will particles do when they strike the plane

    Plane      *m_pNext;             // Next plane in list
};

struct Particle
{
    ArnVec3 m_vCurPos;    // Current position of particle
    ArnVec3 m_vCurVel;    // Current velocity of particle
    float       m_fInitTime;  // Time of creation of particle

    Particle   *m_pNext;      // Next particle in list
};

// Custom vertex and FVF declaration for point sprite vertex points
struct PointVertex
{
    ArnVec3 posit;
    D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
};

//-----------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-----------------------------------------------------------------------------

// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

//-----------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------

/**
@brief 파티클 시스템을 관리하는 클래스 (구현 중)
*/
class CParticleSystem
{

public:

    CParticleSystem(void);
   ~CParticleSystem(void);

    void SetMaxParticles( DWORD dwMaxParticles ) { m_dwMaxParticles = dwMaxParticles; }
	DWORD GetMaxParticles( void ) { return m_dwMaxParticles; }

    void SetNumToRelease( DWORD dwNumToRelease ) { m_dwNumToRelease = dwNumToRelease; }
	DWORD GetNumToRelease( void ) { return m_dwNumToRelease; }

    void SetReleaseInterval( float fReleaseInterval ) { m_fReleaseInterval = fReleaseInterval; }
    float GetReleaseInterval( void ) { return m_fReleaseInterval; }

    void SetLifeCycle( float fLifeCycle ) { m_fLifeCycle = fLifeCycle; }
	float GetLifeCycle( void ) { return m_fLifeCycle; }

    void SetSize( float fSize ) { m_fSize = fSize; }
	float GetSize( void ) { return m_fSize; }
	float GetMaxPointSize( void ) { return m_fMaxPointSize; }

    void SetColor( D3DXCOLOR clrColor ) { m_clrColor = clrColor; }
	D3DXCOLOR GetColor( void ) { return m_clrColor; }

    void SetPosition( ArnVec3 vPosition ) { m_vPosition = vPosition; }
	ArnVec3 GetPosition( void ) { return m_vPosition; }

    void SetVelocity( ArnVec3 vVelocity ) { m_vVelocity = vVelocity; }
	ArnVec3 GetVelocity( void ) { return m_vVelocity; }

    void SetGravity( ArnVec3 vGravity ) { m_vGravity = vGravity; }
	ArnVec3 GetGravity( void ) { return m_vGravity; }

    void SetWind( ArnVec3 vWind ) { m_vWind = vWind; }
	ArnVec3 GetWind( void ) { return m_vWind; }

    void SetAirResistence( bool bAirResistence ) { m_bAirResistence = bAirResistence; }
	bool GetAirResistence( void ) { return m_bAirResistence; }

    void SetVelocityVar( float fVelocityVar ) { m_fVelocityVar = fVelocityVar; }
	float GetVelocityVar( void ) { return m_fVelocityVar; }

    void SetCollisionPlane( ArnVec3 vPlaneNormal, ArnVec3 vPoint, 
                            float fBounceFactor = 1.0f, int nCollisionResult = CR_BOUNCE );

    HRESULT Init( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT Update( float fElapsedTime );
    HRESULT Render( LPDIRECT3DDEVICE9 pd3dDevice );

    void SetTexture( char *chTexFile );
    LPDIRECT3DTEXTURE9 &GetTextureObject();

    HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT InvalidateDeviceObjects(void);
    
	void RestartParticleSystem(void);

private:

    DWORD       m_dwVBOffset;
    DWORD       m_dwFlush;
    DWORD       m_dwDiscard;
    Particle   *m_pActiveList;
    Particle   *m_pFreeList;
    Plane      *m_pPlanes;
	DWORD       m_dwActiveCount;
	float       m_fCurrentTime;
	float       m_fLastUpdate;

    float       m_fMaxPointSize;
    bool        m_bDeviceSupportsPSIZE;

    LPDIRECT3DVERTEXBUFFER9 m_pVB;          // Vertex buffer for point sprites
    LPDIRECT3DTEXTURE9      m_ptexParticle; // Particle's texture
    
    // Particle Attributes
    DWORD       m_dwMaxParticles;
    DWORD       m_dwNumToRelease;
    float       m_fReleaseInterval;
    float       m_fLifeCycle;
    float       m_fSize;
    D3DXCOLOR   m_clrColor;
    ArnVec3 m_vPosition;
    ArnVec3 m_vVelocity;
    ArnVec3 m_vGravity;
    ArnVec3 m_vWind;
    bool        m_bAirResistence;
    float       m_fVelocityVar;
    char       *m_chTexFile;
};

#endif /* CPARTICLESYSTEM_H_INCLUDED */