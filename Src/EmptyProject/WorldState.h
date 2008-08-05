#pragma once
#include "state.h"
#include "ArnFile.h"
#include "ArnSceneGraph.h"
#include "ArnNode.h"
#include "Picture.h"
#include "PictureMap.h"
#include "Sound.h"
#include "Unit.h"
#include "VideoMan.h"

typedef std::set<Unit*> UnitSet;

class WorldState : public State
{
public:
	WorldState(void);
	~WorldState(void);

	virtual HRESULT enter();
	virtual HRESULT leave();

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove(double fTime, float fElapsedTime);

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT release();

	void addUnit(Unit* u);

private:
	void setupLight();

	ArnFileData*					m_afd;
	ArnSceneGraph*					m_sg;
	PictureMap						m_pic;
	Picture							m_picRhw;
	Picture							m_picSmiley;
	Picture							m_avatar;
	Sound							m_sound;

	LPDIRECT3DVERTEXSHADER9         m_pVertexShader;
	LPD3DXCONSTANTTABLE             m_pConstantTable;
	LPDIRECT3DVERTEXDECLARATION9    m_pVertexDeclaration;

	Unit*							m_heroUnit;
	float							m_sampleTeapotMeshRot;
	LPD3DXMESH						m_aTile;

	double							m_startTime;

	
	UnitSet							m_unitSet;
};
