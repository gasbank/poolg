#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeMaterial1;
struct NodeMaterial2;

class ArnMaterial : public ArnNode
{
public:
	typedef std::vector<LPDIRECT3DTEXTURE9> TextureList;
	ArnMaterial();
	~ArnMaterial(void);
	
	static ArnNode*				createFrom(const NodeBase* nodeBase);
	unsigned int				getMaterialCount() const { return m_materialCount; }
	const D3DMATERIAL9&			getD3DMaterialData() const { return m_data.m_d3dMaterial; }
	unsigned int				getTexImgCount() const { return m_data.m_texImgList.size(); }
	const STRING&				getTexImgName(unsigned int idx) const { return m_data.m_texImgList[idx]; }
	LPDIRECT3DTEXTURE9			getD3DTexture(unsigned int idx) const { if (idx < m_d3dTextureList.size()) return m_d3dTextureList[idx]; else return 0; }
private:
	void						buildFrom(const NodeMaterial1* nm);
	void						buildFrom(const NodeMaterial2* nm);

	unsigned int m_materialCount;
	MaterialData m_data;
	TextureList m_d3dTextureList;
};
