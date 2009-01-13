#pragma once

struct NodeBase
{
	virtual ~NodeBase() {}

	NODE_DATA_TYPE m_ndt;
	char* m_nodeName;
	unsigned int m_nodeChunkSize;
};

struct NodeUnidentified : public NodeBase
{
	int m_dummy;
};

struct NodeMaterial1 : public NodeBase
{
	unsigned int m_materialCount;
};
struct NodeMaterial2 : public NodeBase
{
	char* m_parentName;
	char* m_materialName;
	D3DMATERIAL9* m_d3dMaterial;
	unsigned int m_texCount;
	std::vector<char*> m_texNameList;
};
struct NodeMesh2 : public NodeBase
{
	~NodeMesh2() { delete [] m_mtds; }

	unsigned int m_meshVerticesCount;
	ARN_VDD* m_vdd;
	unsigned int m_meshFacesCount;
	unsigned int* m_triangleIndice;
	unsigned int* m_materialRefs;
	unsigned int m_materialCount;
	ARN_MTD_Data* m_mtds;
};

struct NodeMesh3 : public NodeBase
{
	const char* m_parentName;
	const char* m_ipoName;
	const D3DXMATRIX* m_localXform;
	BOOL m_bArmature;
	//D3DXMATRIX* m_unusedXform;
	unsigned int m_materialCount;
	unsigned int m_meshVerticesCount;
	unsigned int m_meshFacesCount;
	const DWORD* m_attrToMaterialMap; // unused
	std::vector<const char*> m_matNameList;
	const ArnVertex* m_vertex;
	const unsigned short* m_faces;
	const DWORD* m_attr;
	const char* m_armatureName;
	std::vector<Bone2> m_bones;

	std::vector<const char*> m_boneMatIdxMap;
	float* m_weights; // Start address of v0[w0, w1, w2] v1[w0, w1, w2] ... w? are floats
	unsigned char* m_matIdx; // v0[m0, m1, m2, m3], v1[m0, m1, m2, m3] ... m? are unsigned chars(0~255)

};

struct NodeAnim1 : public NodeBase
{
	unsigned int m_keyCount;
	RST_DATA* m_rstKeys;
};

struct NodeBone1 : public NodeBase
{
	D3DMATRIX* m_offsetMatrix;
	unsigned int m_infVertexCount; // influencing vertex count
	unsigned int* m_vertexIndices;
	float* m_weights;
};

struct NodeBone2 : public NodeBase
{
	const char* m_parentBoneName;
	D3DMATRIX* m_offsetMatrix;
	unsigned int m_infVertCount;
	BoneIndWeight* m_indWeightArray;
};

struct NodeSkeleton1 : public NodeBase
{
	char* m_associatedMeshName;
	unsigned int m_maxWeightsPerVertex;
	unsigned int m_boneCount;
};

struct NodeHierarchy2 : public NodeBase
{
	const char* m_parentName;
	unsigned int m_boneCount;
	std::vector<Bone2> m_bones;
};


struct NodeHierarchy1 : public NodeBase
{
	~NodeHierarchy1()
	{
		delete [] m_frames;
	}
	unsigned int m_frameCount;
	MyFrameDataShell* m_frames;
};

struct NodeLight1 : public NodeBase
{
	D3DLIGHT9* m_light;
};
struct NodeLight2 : public NodeBase
{
	char* m_parentName;
	char* m_ipoName;
	D3DXMATRIX* m_localXform;
	D3DLIGHT9* m_light;
};
struct NodeCamera1 : public NodeBase
{
	ARN_NDD_CAMERA_CHUNK* m_camera;
};
struct NodeCamera2 : public NodeBase
{
	enum CamType { CT_ORTHO = 1, CT_PERSP = 0 };
	char* m_parentName;
	char* m_ipoName;
	D3DXMATRIX* m_localXform;
	CamType m_camType;
	float m_angle;
	float m_clipStart;
	float m_clipEnd;
	float m_scale;
};

struct NodeIpo1 : public NodeBase
{
	unsigned int m_ipoCount;
};
struct NodeIpo2 : public NodeBase
{
	~NodeIpo2() { delete [] m_curves; }

	char* m_parentName;
	unsigned int m_curveCount;
	CurveDataShell* m_curves;
};
struct NodeAction1 : public NodeBase
{
	unsigned int m_actionCount;
	std::vector<std::pair<const char*, std::vector<std::pair<const char*, const char*> > > > m_actions;
};

struct NodeSymLink1 : public NodeBase
{
	int dummy;
};
//////////////////////////////////////////////////////////////////////////

struct ArnBinaryFile
{
	ArnBinaryFile() : m_data(0) {}

	unsigned int m_fileSize;
	char* m_data;
	unsigned int m_curPos;
};

struct ArnFileData
{
	char* m_fileDescriptor;
	unsigned int m_nodeCount;
	typedef std::vector<NodeBase*> NodeList;
	NodeList m_nodes;
	char* m_terminalDescriptor;

	ArnBinaryFile m_file;
};



void load_arnfile(const TCHAR* fileName, ArnFileData& afd);
void release_arnfile(ArnFileData& afd);

void parse_node(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeUnidentified(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeMaterial1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeMaterial2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeMesh2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeMesh3(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeSkeleton1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeHierarchy2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeHierarchy1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeLight1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeLight2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeCamera1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeCamera2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeAnim1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeBone1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeBone2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeIpo1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeIpo2(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeAction1(ArnBinaryFile& abf, NodeBase*& nodeBase);
void parse_nodeSymLink(ArnBinaryFile& abf, NodeBase*& nodeBase);

//////////////////////////////////////////////////////////////////////////

void			file_load(const TCHAR* fileName, ArnBinaryFile& file);
void			file_unload(ArnBinaryFile& file);


char*			file_read_string(ArnBinaryFile& file);
unsigned int	file_read_uint(ArnBinaryFile& file);
unsigned int*	file_read_uint_array(ArnBinaryFile& file, unsigned int count);
int				file_read_int(ArnBinaryFile& file);
float			file_read_float(ArnBinaryFile& file);
BOOL			file_read_BOOL(ArnBinaryFile& file);
void*			file_read_implicit_array(ArnBinaryFile& file, unsigned int byteLen);

ARN_VDD*		file_read_arn_vdd_array(ArnBinaryFile& file, unsigned int count);
ARN_MTD*		file_read_arn_mtd_array(ArnBinaryFile& file, unsigned int count);


template<class T>
T* file_read(ArnBinaryFile& file, unsigned int count = 1)
{
	if (count == 0)
		throw std::runtime_error("Unacceptable parameter");
	T* t = (T*)(file.m_data + file.m_curPos);
	file.m_curPos += sizeof(T) * count;
	return t;
}