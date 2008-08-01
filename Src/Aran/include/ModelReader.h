#pragma once



// Hierarchy info
struct MyFrame : public D3DXFRAME
{
	MyFrame():isRoot(FALSE),sibling(-1),firstChild(-1)
	{
		this->Name = this->nameFixed;
		ZeroMemory(&combinedMatrix, sizeof(D3DXMATRIX));
		ZeroMemory(&TransformationMatrix, sizeof(D3DXMATRIX));
	}
	~MyFrame() {}
	char		nameFixed[128];
	BOOL		isRoot;
	D3DXMATRIX	combinedMatrix;
	size_t		sibling;
	size_t		firstChild;
};

struct Bone : public BoneData
{
	Bone()
	{
		translationKeys = scaleKeys = 0;
		rotationKeys = 0;
		translationKeysSize = scaleKeysSize = rotationKeysSize = 0;
	}
	~Bone()
	{
		delete [] translationKeys; translationKeys = 0; translationKeysSize = 0;
		delete [] rotationKeys; rotationKeys = 0; rotationKeysSize = 0;
		delete [] scaleKeys; scaleKeys = 0; scaleKeysSize = 0;
	}

	// Basic data is moved to BoneData (superclass)

	std::vector<RST_DATA>	keys; // keyframe animation data in ARN file
	
	// keyframe animation data of ID3DXKeyframedAnimationSet
	// this->keys should be converted to following data format using ModelReader::AllocateAsAnimationSetFormat()
	LPD3DXKEY_VECTOR3		translationKeys;
	LPD3DXKEY_VECTOR3		scaleKeys;
	LPD3DXKEY_QUATERNION	rotationKeys;
	UINT					translationKeysSize, scaleKeysSize, rotationKeysSize;
};

// DEPRECATED: use SkeletonData instead
struct SkeletonNode
{
	char				nameFixed[128];
	char				associatedMeshName[128];
	int					maxWeightsPerVertex; // same as max bones per vertex
	int					bonesCount;
	std::vector<Bone>	bones;
};

//////////////////////////////////////////////////////////////////////////

class ModelReader
{

public:
	ModelReader();
	ModelReader(LPDIRECT3DDEVICE9 lpDev, DWORD fvf);
	~ModelReader(void);

	HRESULT			Initialize(LPDIRECT3DDEVICE9 lpDev, DWORD fvf,
		HWND hLoadingWnd, const TCHAR* fileName, LPD3DXANIMATIONCONTROLLER lpAC, const BOOL initAC = FALSE );
	void			SetDev(LPDIRECT3DDEVICE9 lpDev);
	void			SetFVF(DWORD fvf);
	void			SetLoadingWindowHandle(HWND hLoadingWnd);

	// Read mesh(vertices, vertex indices, texture coords, ...), materials, animations
	// ARN1x, ARN2x compatible
	int				Read(const TCHAR* fileName);

	//static const int vddSize = sizeof(ARN_VDD);
	
	// (0) find lpMeshes[meshIndex] and corresponding skeletonNode element
	// (1) implement ID3DXSkinInfo using the skeletonNode element (no hierarchy included)
	// (2) call ConvertToIndexedBlendedMesh()
	// (3) return LPD3DXMESH to this->lpSkinnedMeshes
	int				BuildBlendedMeshByMeshIndex(int meshIndex);

	// connect interconnection pointers of this->hierarchy using array index reference
	int				BuildBoneHierarchyByMeshIndex(int meshIndex);
	
	// keyframeEndIndex means there the last frame;
	// i.e. default argument will build animation set of entire frames
	HRESULT			BuildKeyframedAnimationSetOfSkeletonNodeIndex( int skeletonNodeIndex, int keyFrameStartIndex = 0, int keyFrameEndIndex = -1);

	static int		AllocateAsAnimationSetFormat(UINT sourceArraySize, RST_DATA* sourceArray,
		UINT* pScaleSize, UINT* pRotationSize, UINT* pTranslationSize,
		D3DXKEY_VECTOR3** ppScale, D3DXKEY_QUATERNION** ppRotation, D3DXKEY_VECTOR3** ppTranslation,
		BOOL removeDuplicates = FALSE);
	
	// update combined transformation matrix hierarchically
	void			UpdateBoneCombinedMatrixByMeshIndex(int meshIndex);
	void			UpdateBoneCombinedMatrixRecursive(MyFrame* startFrame, D3DXMATRIX& parentCombinedTransform);
	
	void			clearMembers();
	void			SetFileName( const TCHAR* fileName );
	HRESULT			AdvanceTime(float timeDelta) const; // redirection to AC

	// GET methods
	DWORD			GetFVF() const;
	LPDIRECT3DVERTEXBUFFER9 GetVB() const;
	int				GetVBLength() const;
	int				GetTotalFaceCount() const;
	int				GetMaterialReference(int meshIndex, int faceIndex) const;
	int				GetMaterialReferenceFast(int meshIndex, int materialIndex) const;
	const D3DMATERIAL9* GetMaterial(int referenceIndex) const;
	int				GetNotIndMeshCount() const;
	int				GetIndMeshCount() const;
	int				GetFaceCount(int meshIndex) const;
	LPDIRECT3DTEXTURE9 GetTexture(int referenceIndex) const;
	int				GetMaterialCount(int meshIndex) const;
	int				GetAnimQuatSize(int meshIndex) const;
	RST_DATA*		GetAnimQuat(int meshIndex) const;
	int				GetLightCount() const;
	D3DLIGHT9		GetLight(int i) const;
	EXPORT_VERSION	GetExportVersion() const;
	LPD3DXKEYFRAMEDANIMATIONSET GetKeyframedAnimationSet( int animSetIndex = 0 ) const;
	MyFrame*		GetFrameRootByMeshIndex(int meshIndex);
	MyFrame*		GetFrameBySkeletonName(const char* skelName); // for testing
	LPD3DXMESH		GetMeshPointer(int i) const;
	LPD3DXMESH		GetSkinnedMeshPointer(int i) const;
	LPD3DXSKININFO	GetSkinInfoPointer(int i) const;
	const SkeletonNode* GetSkeletonNodePointer(int index) const;
	size_t			GetSkeletonNodeSize() const;
	int				GetMeshIndexBySkeletonIndex(int skelIndex) const;
	int				GetSkeletonIndexByMeshIndex(int meshIndex) const;
	const D3DXMATRIX* GetTransformationMatrixByBoneName( const char* boneName ) const;
	D3DXMATRIX*		GetCombinedMatrixByBoneName( const char* boneName );
	ArnNodeHeader	GetArnNodeHeader(int idx);
	size_t			GetArnNodeHeadersSize();
	const D3DXMATRIX* GetAnimMatControlledByAC(int meshIndex) const;
	const TCHAR*	GetFileNameOnly();
	ARN_NDD_CAMERA_CHUNK* GetFirstCamera();
	BOOL			IsInitialized() const;
	const LPD3DXANIMATIONCONTROLLER GetAnimationController() const { return this->lpAnimationController; }


private:
	int				BuildTopLevelNodeList();

	int				ParseNDD_Skeleton(int nodeHeaderIndex);
	int				ParseNDD_Mesh1(int nodeHeaderIndex);
	int				ParseNDD_Mesh2(int nodeHeaderIndex);
	HRESULT			ParseNDD_Anim(NODE_DATA_TYPE belongsToType, Bone* pBone); // child NDD structure
	int				ParseNDD_Hierarchy(int nodeHeaderIndex);
	int				ParseNDD_Light(int nodeHeaderIndex);
	int				ParseNDD_Camera(int nodeHeaderIndex);


	TCHAR							szFileName[64];
	BOOL							initialized;
	LPDIRECT3DDEVICE9				lpDev;
	LPDIRECT3DVERTEXBUFFER9			lpVB;
	DWORD							fvf;
	// TODO: global or local animation controller?
	// Use local animation controller to model support instancing
	BOOL							useLocalAC;
	LPD3DXANIMATIONCONTROLLER		lpAnimationController;
	int								notIndVertTotalSize; // Vertex buffer size in bytes (not indexed)
	int								indVertTotalSize; // Vertex buffer size in bytes (indexed); cumulative value through all this->lpMeshes's VB
	int								lightCount;
	int								notIndTotalMeshCount; // not indexed mesh count
	int								indTotalMeshCount; // indexed mesh count == element count of lpMeshes
	int								skinnedMeshCount; // element count of lpSkinnedMeshes
	int								nodeCount; // the value read from the ARN file
	std::vector<ARN_MTD>			materialList;
	std::vector<LPDIRECT3DTEXTURE9> textureList;
	std::vector<int*>				materialReference;
	std::vector<int>				meshVertCount; // vertex count per mesh
	std::vector<int>				materialCount;
	int								totalFaceCount;
	std::vector<int*>				materialReferenceFast;
	std::vector<int>				animQuatSize;
	std::vector<RST_DATA*>			animQuat; // animation data per NDT_MESH1, NDT_MESH2
	std::vector<D3DXMATRIX>			animMatControlledByAC;
	std::vector<RST_DATA*>			lightAnimQuat; // animation data per light
	std::vector<D3DLIGHT9>			lights;
	HWND							hLoadingWnd;
	EXPORT_VERSION					exportVersion; // ARN10, ARN11, ARN20, ...
	std::vector<STRING>		notIndMeshNames;

	// for ARN2x format
	std::vector<STRING>		indMeshNames;
	LPD3DXMESH*						lpMeshes;
	std::vector<LPD3DXMESH>			lpSkinnedMeshes;
	std::vector<LPD3DXSKININFO>		lpSkinnedMeshesSkinInfo;
	D3DXMESHCONTAINER				meshContainer;
	D3DXFRAME						frame;

	int								hierarchySize;
	std::vector<MyFrame>			hierarchy;
	std::vector<SkeletonNode>		skeletonNode;
	LPD3DXKEYFRAMEDANIMATIONSET		lpKeyframedAnimationSet;
	std::vector<LPD3DXKEYFRAMEDANIMATIONSET> lpKeyframedAnimationSetList;

	std::vector<ArnNodeHeader>		nodeHeaders;
	typedef std::map<NODE_DATA_TYPE, unsigned int> NodeTypeMap;
	NodeTypeMap						nodeTypeCounter;

	std::ifstream					fin;

	std::vector<int>				fmtOffset;
	std::vector<int>				fmtLength; // equals faceCount(=vertCount / 3)

	// ARN10, ARN11 only
	std::vector<int>				verticesOffset;
	std::vector<int>				verticesLength; // equals vertCount * Vertex(fvf) Size


	// Camera
	std::vector<ARN_NDD_CAMERA_CHUNK> cameraNodes;

};



