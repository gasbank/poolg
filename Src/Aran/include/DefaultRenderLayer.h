#pragma once
#include "renderlayer.h"

class Character;
class ArnObject;
class ArnSceneGraph;
struct ArnFileData;

class DefaultRenderLayer :
	public RenderLayer
{
public:
	DefaultRenderLayer(Character* pChar);
	~DefaultRenderLayer(void);

	virtual HRESULT render(double fTime, float fElapsedTime);

private:
	Character* m_pChar;
};

//////////////////////////////////////////////////////////////////////////

class BoxRenderLayer : public RenderLayer
{
public:
	BoxRenderLayer();
	~BoxRenderLayer();

	virtual HRESULT render(double fTime, float fElapsedTime);

private:
	
	TCHAR m_debugBuffer[512];
	D3DXMATRIX m_matWorld, m_matView, m_matProjection;

	std::vector<ArnObject*> m_objects;

	LPD3DXMESH m_testMesh;

	ArnSceneGraph* m_simpleSG;
	ArnFileData* m_arnFileData;
};

LPD3DXMESH newTestPlaneMesh(float width, float height, int segWidth, int segHeight);
