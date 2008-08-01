#pragma once
#include "ArnXformable.h"

struct NodeBase;
struct NodeLight1;
struct NodeLight2;

class ArnLight : public ArnXformable
{
public:
	ArnLight();
	~ArnLight(void);
	
	static ArnNode*		createFrom(const NodeBase* nodeBase);
	const D3DLIGHT9&	getD3DLightData() const { return m_d3dLight; }

	// *** INTERNAL USE ONLY START ***
	virtual void		interconnect(ArnNode* sceneRoot);
	// *** INTERNAL USE ONLY END ***
private:
	void				buildFrom(const NodeLight1* nl);
	void				buildFrom(const NodeLight2* nl);

	D3DLIGHT9			m_d3dLight;
};
