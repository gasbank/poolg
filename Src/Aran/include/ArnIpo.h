#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeIpo1;
struct NodeIpo2;

class ArnIpo : public ArnNode
{
public:
	ArnIpo(void);
	~ArnIpo(void);

	static ArnNode*				createFrom(const NodeBase* nodeBase);
	unsigned int				getIpoCount() const { return m_ipoCount; }
	unsigned int				getCurveCount() const { return m_curveCount; }
	const CurveData&			getCurveData(unsigned int idx) const { return m_curves[idx]; }
	LPD3DXKEYFRAMEDANIMATIONSET getD3DXAnimSet() const { return m_d3dxAnimSet; }
	DWORD						getCurveNames() const { return m_curveNames; }

	// *** INTERNAL USE ONLY START ***
	virtual void				interconnect(ArnNode* sceneRoot);
	// *** INTERNAL USE ONLY END ***

	static const unsigned int	FPS = 60;
	static CurveName			CurveNameStrToEnum(const char* name);
private:
	void						buildFrom(const NodeIpo1* ni);
	void						buildFrom(const NodeIpo2* ni);

	DWORD						m_curveNames;
	
	unsigned int				m_ipoCount;
	unsigned int				m_curveCount;
	std::vector<CurveData>		m_curves;
	LPD3DXKEYFRAMEDANIMATIONSET m_d3dxAnimSet;
};
