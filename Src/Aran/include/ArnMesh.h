#pragma once
#include "ArnXformable.h"

struct NodeBase;
struct NodeMesh2;
struct NodeMesh3;
class ArnMaterial;
class ArnHierarchy;

class ArnMesh : public ArnXformable
{
public:
	typedef std::vector<ArnMaterial*> MaterialRefList;

	ArnMesh();
	~ArnMesh(void);

	static ArnNode*		createFrom(const NodeBase* nodeBase);

	const LPD3DXMESH&	getD3DXMesh() { return m_d3dxMesh; }
	void				setD3DXMesh(const LPD3DXMESH d3dxMesh) { m_d3dxMesh = d3dxMesh; }
	const MeshData&		getMeshData() const { return m_data; }
	const D3DMATERIAL9*	getMaterial(unsigned int i) const;
	ArnMaterial*		getMaterialNode(unsigned int i) const { return m_materialRefList[i]; }
	bool				isVisible() const { return m_bVisible; }
	void				setVisible(bool val) { m_bVisible = val; }
	bool				isCollide() const { return m_bCollide; }
	void				setCollide(bool val) { m_bCollide = val; }

	// *** INTERNAL USE ONLY START ***
	virtual void		interconnect(ArnNode* sceneRoot);
	// *** INTERNAL USE ONLY END ***
private:
	void				buildFrom(const NodeMesh2* nm);
	void				buildFrom(const NodeMesh3* nm);

	LPD3DXMESH			m_d3dxMesh;
	LPDIRECT3DVERTEXBUFFER9 m_d3dvb;
	LPDIRECT3DINDEXBUFFER9 m_d3dib;

	MaterialRefList		m_materialRefList;
	MeshData			m_data;

	bool				m_bVisible;
	bool				m_bCollide;

	ArnHierarchy*		m_skeleton;

	struct BoneDataInternal
	{
		STRING									nameFixed;
		std::vector<std::pair<DWORD, float> >	indWeight;
	};
	std::vector<BoneDataInternal>	m_boneDataInt;
};

HRESULT arn_build_mesh(IN LPDIRECT3DDEVICE9 dev, IN const NodeMesh2* nm, OUT LPD3DXMESH& mesh);
HRESULT arn_build_mesh(IN LPDIRECT3DDEVICE9 dev, IN const NodeMesh3* nm, OUT LPD3DXMESH& mesh);
HRESULT arn_build_mesh(IN LPDIRECT3DDEVICE9 dev, IN const NodeMesh3* nm, OUT LPDIRECT3DVERTEXBUFFER9& d3dvb, OUT LPDIRECT3DINDEXBUFFER9& d3dib);