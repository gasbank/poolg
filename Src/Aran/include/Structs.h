// Structs.h
// 2007, 2008 Geoyeob Kim (gasbank@gmail.com)
#ifndef __STRUCTS_H_
#define __STRUCTS_H_

enum
{
	ARNVERTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL
};

enum EXPORT_VERSION
{
	EV_UNDEFINED,
	EV_ARN10,
	EV_ARN11,
	EV_ARN20,
	EV_ARN25,
	EV_FORCE_DWORD = 0x7fffffff,
};

enum NODE_DATA_TYPE // or NDD_DATA_TYPE
{
	NDT_UNKNOWN,
	
	NDT_CONTAINER1			= 0x1000,
	NDT_CONTAINER2,
	NDT_CONTAINER3,
	NDT_CONTAINER4,
	NDT_CONTAINER5,

	NDT_MESH1				= 0x2000,	// ARN10, ARN11
	NDT_MESH2,							// ARN20
	NDT_MESH3,							// ARN25
	NDT_MESH4,
	NDT_MESH5,

	NDT_LIGHT1				= 0x3000,	// ARN10, ARN11, ARN20
	NDT_LIGHT2,							// ARN25
	NDT_LIGHT3,
	NDT_LIGHT4,
	NDT_LIGHT5,

	NDT_SKELETON1			= 0x4000,	// ARN10, ARN11, ARN20
	NDT_SKELETON2,
	NDT_SKELETON3,
	NDT_SKELETON4,
	NDT_SKELETON5,

	NDT_HIERARCHY1			= 0x5000,	// ARN10, ARN11, ARN20
	NDT_HIERARCHY2,
	NDT_HIERARCHY3,
	NDT_HIERARCHY4,
	NDT_HIERARCHY5,

	NDT_ANIM1				= 0x6000,	// ARN10, ARN11, ARN20
	NDT_ANIM2,
	NDT_ANIM3,
	NDT_ANIM4,
	NDT_ANIM5,
	
	NDT_CAMERA1				= 0x7000,	// ARN10, ARN11, ARN20
	NDT_CAMERA2,						// ARN25
	NDT_CAMERA3,
	NDT_CAMERA4,
	NDT_CAMERA5,

	NDT_BONE1				= 0x8000,	// ARN10, ARN11, ARN20
	NDT_BONE2,
	NDT_BONE3,
	NDT_BONE4,
	NDT_BONE5,
	
	NDT_MATERIAL1			= 0x9000,	// ARN25 : Global materials node which consists of NDT_MATERIAL2
	NDT_MATERIAL2,						// ARN25 : Individual material data node
	NDT_MATERIAL3,
	NDT_MATERIAL4,
	NDT_MATERIAL5,

	NDT_IPO1				= 0xa000,	// ARN25 : Global IPOs node which consists of NDT_IPO2
	NDT_IPO2,							// ARN25 : Individual IPO data node
	NDT_IPO3,
	NDT_IPO4,
	NDT_IPO5,
	
	NDT_SYMLINK1			= 0xb000,	// ARN25 : Symbolic link to another node
	NDT_SYMLINK2,
	NDT_SYMLINK3,
	NDT_SYMLINK4,
	NDT_SYMLINK5,

	// Runtime objects RTTI
	NDT_RT_CONTAINER		= 0x10000000,
	NDT_RT_MESH,
	NDT_RT_CAMERA,
	NDT_RT_LIGHT,
	NDT_RT_ANIM,
	NDT_RT_MATERIAL,
	NDT_RT_HIERARCHY,
	NDT_RT_SKELETON,
	NDT_RT_BONE,
	NDT_RT_IPO,
	NDT_RT_SYMLINK,
	NDT_RT_SCENEGRAPH,
	
	NDT_FORCE_DWORD = 0x7fffffff,
};

//////////////////////////////////////////////////////////////////////////

class MY_CUSTOM_MESH_VERTEX
{
public:
	MY_CUSTOM_MESH_VERTEX() {}
	MY_CUSTOM_MESH_VERTEX(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		this->x = x; this->y = y; this->z = z;
		this->nx = nx; this->ny = ny; this->nz = nz;
		this->u = u; this->v = v;
	}
	float x, y, z, nx, ny, nz, u, v;
	static const DWORD MY_CUSTOM_MESH_VERTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

struct ArnVertex
{
	float x, y, z, nx, ny, nz, u, v;

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

struct BezTripleData
{
	float vec[3][2];
	//float alfa, weight, radius;	/* alfa: tilt in 3D View, weight: used for softbody goal weight, radius: for bevel tapering */
	//short h1, h2; 				/* h1, h2: the handle type of the two handles */
	//char f1, f2, f3, hide;		/* f1, f2, f3: used for selection status,  hide: used to indicate whether BezTriple is hidden */
};

enum CurveType { IPO_CONST, IPO_LIN, IPO_BEZ };
enum CurveName
{
	CN_LocX		= 0x00000001,
	CN_LocY		= 0x00000002,
	CN_LocZ		= 0x00000004,
	CN_ScaleX	= 0x00000010,
	CN_ScaleY	= 0x00000020,
	CN_ScaleZ	= 0x00000040,
	CN_RotX		= 0x00000100,
	CN_RotY		= 0x00000200,
	CN_RotZ		= 0x00000400,

	CN_SIZE = 9,
	CN_UNKNOWN = 0x7fffffff, // error
};
struct CurveDataShell
{
	char* name;
	unsigned int pointCount;
	CurveType type;
	BezTripleData* points;
};
class CurveData
{
public:
	STRING nameStr;
	CurveName name;
	unsigned int pointCount;
	CurveType type;
	std::vector<BezTripleData> points;
};

class MaterialData
{
public:
	STRING m_materialName;
	D3DMATERIAL9 m_d3dMaterial;
	std::vector<STRING> m_texImgList;
};
struct SkeletonData
{
	STRING				name;
	STRING				associatedMeshName;
	unsigned int		maxWeightsPerVertex; // same as max bones per vertex
	unsigned int		bonesCount;
};
struct BoneData
{
	virtual ~BoneData() {}

	STRING					nameFixed;
	D3DXMATRIX				offsetMatrix;
	unsigned int			infVertexCount;
	std::vector<DWORD>		indices;
	std::vector<float>		weights;
};

struct MyFrameDataShell
{
	char*	m_frameName;
	BOOL	m_rootFlag;
	int		m_sibling;
	int		m_firstChild;
};
struct MyFrameData
{
	STRING	m_frameName;
	BOOL	m_rootFlag;
	int		m_sibling;
	int		m_firstChild;
};
class MeshData
{
public:
	unsigned int vertexCount;
	unsigned int faceCount;
	unsigned int materialCount;
	std::vector<STRING> matNameList;
};
//////////////////////////////////////////////////////////////////////////
// Unions
//////////////////////////////////////////////////////////////////////////


struct POINT3FLOAT
{
	float x, y, z;
	static const POINT3FLOAT ZERO;
};

struct POINT4FLOAT
{
	float x, y, z, w;
	static const POINT4FLOAT ZERO;
};

// Vertex Data Definition (VDD) for ARN format
// (Applied format: ARN10, ARN20)
// Point3 ---> Point3Simple type simplification (to remove 3ds Max 9 .h file dependency when compiling game engine)
typedef union tagTEXCOORD{
	float xy[2];
	float uv[2];
	
} TEXCOORD, POINT2FLOAT;

typedef union tagARN_KDD // Key Data Definition
{
	float rst[4+3+3]; // rot 4 + scale 3 + trans 3
} ARN_KDD;







//////////////////////////////////////////////////////////////////////////
// ModelExporter (classic)

typedef struct tagARN_VDD
{
	POINT3FLOAT vertex;
	POINT3FLOAT normal;
	union
	{
		DWORD color; // vertex color (may not be used)
	};
	
	TEXCOORD tc;

	static const DWORD ARN_VDD_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
} ARN_VDD;

typedef struct tagARN_VDD_S
{
	POINT3FLOAT pos, normal, tangent, binormal, uvw, color;
	DWORD boneIndices; // packed by four-unsigned-byte
	float boneWeights[4];

} ARN_VDD_S;

// Material & Texture Definition (MTD) for ARN format
// (STRING should be converted to 0-terminated char[])
typedef struct tagARN_MTD
{
	STRING strMatName;
	D3DMATERIAL9 d3dMat;
	STRING strTexFileName;
} ARN_MTD;

struct ARN_MTD_Data
{
	char* m_strMatName;
	D3DMATERIAL9* m_d3dMat;
	char* m_strTexFileName;
};

typedef struct tagARN_NDD_HEADER
{
	NODE_DATA_TYPE ndt;
	char* uniqueName;
	int chunkSize;

} ARN_NDD_HEADER;


typedef struct tagARN_NDD_ANIM1
{
	ARN_NDD_HEADER header;

	int keyframeSize;
	ARN_KDD* pKeyframe;

} ARN_NDD_ANIM1;

typedef struct tagARN_NDD_MESH2
{
	ARN_NDD_HEADER header;

	int vertexSize;
	ARN_VDD* pVertex;

	int faceSize;
	int (*pIndex)[3];

	int* fmt; // Face-Material Table

	int mtdSize; // Material & Texture Definition size
	ARN_MTD* pMTD;

	ARN_NDD_ANIM1 anim;

} ARN_NDD_MESH2;

typedef struct tagARN_NDD_MESH3
{
	ARN_NDD_HEADER header;

	int vertexSize;
	int indexSize;

	ARN_MTD mtd;
	int boneSize;
	int frameSize;

	ARN_VDD_S* pVertex;
	int (*pIndex)[3];
	float (*pAnimKey)[16]; // matrix form

} ARN_NDD_MESH3;

typedef struct tagARN_NDD_CAMERA_CHUNK
{

	POINT3FLOAT pos, targetPos;
	POINT4FLOAT rot;

	POINT3FLOAT upVector, lookAtVector;

	float farClip;
	float nearClip;

	// No animation yet
} ARN_NDD_CAMERA_CHUNK;






// Deprecated

struct ArnNodeHeader
{
	NODE_DATA_TYPE ndt;
	STRING uniqueName; // STRING is Multibyte format
	DWORD chunkSize;
	DWORD chunkStartPos;
};


struct RST_DATA
{
	float x, y, z, w; // rotation
	float sx, sy, sz; // scaling
	float tx, ty, tz; // translation

	static const RST_DATA IDENTITY;
};

struct DX_CONSTS
{
	static const D3DXVECTOR3 D3DXVEC3_ZERO;
	static const D3DXVECTOR3 D3DXVEC3_ONE;
	static const D3DXVECTOR3 D3DXVEC3_X;
	static const D3DXVECTOR3 D3DXVEC3_Y;
	static const D3DXVECTOR3 D3DXVEC3_Z;
	static const D3DXQUATERNION D3DXQUAT_IDENTITY;
	static const D3DXMATRIX D3DXMAT_IDENTITY;

	static const D3DXCOLOR D3DXCOLOR_BLACK;
	static const D3DXCOLOR D3DXCOLOR_RED;
	static const D3DXCOLOR D3DXCOLOR_GREEN;
	static const D3DXCOLOR D3DXCOLOR_BLUE;
	static const D3DXCOLOR D3DXCOLOR_YELLOW;
	static const D3DXCOLOR D3DXCOLOR_MAGENTA;
	static const D3DXCOLOR D3DXCOLOR_CYAN;
	static const D3DXCOLOR D3DXCOLOR_WHITE;
};

#endif // #ifndef __STRUCTS_H_
