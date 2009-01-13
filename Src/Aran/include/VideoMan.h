// VideoMan.h
// 2007 Geoyeob Kim
//
// Aran Project Video Manager
//
// (1) Window Creation
// (2) Direct3D 9 Initialization
// (3) Model Rendering Capability
//

#pragma once


#include "InputMan.h"
#include "ModelReader.h"
#include "DungeonInterface.h"
#include "Singleton.h"
#include "PreciseTimer.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if((p)!=0) { (p)->Release(); (p) = 0; }
#endif

class Character;
class ArnCamera;
class ArnNode;
class ArnMesh;

struct MY_COLOR_VERTEX
{
	FLOAT x, y, z;
	DWORD color;
};
//
// Classes and Structures
//
struct ARN_CAMERA
{
	// eye: Position of camera
	// at: Look-at vector
	// up: Up-vector

	D3DXVECTOR3 eye, at, up;
	float farClip, nearClip;
	float angle; // in radian
};

class RenderLayer;

class VideoMan : public Singleton<VideoMan>
{
private:
	InputMan* pInputMan;

	TCHAR szClassName[64];
	WNDCLASS wndClass;
	HWND hWnd, hLoadingWnd;
	int screenWidth, screenHeight;

	LPDIRECT3D9 lpD3D;
	LPDIRECT3DDEVICE9 lpD3DDevice;
	LPDIRECT3DTEXTURE9 lpTex1;

	LPD3DXFONT lpFont;

	D3DXMATRIX matWorld, matView, matProjection;

	DWORD shaderFlags;
	LPD3DXBUFFER lpCompiledFragments;
	LPD3DXFRAGMENTLINKER lpFragmentLinker;
	LPDIRECT3DVERTEXSHADER9 lpVertexShader;
	LPD3DXCONSTANTTABLE lpConstantTable;
	LPD3DXEFFECT lpEffect;
	LPD3DXEFFECT lpEffectSkinning;

	LPD3DXMESH lpCustomMesh, lpCustomSkinnedMesh;
	LPD3DXSKININFO lpSkinInfo;
	D3DXMESHCONTAINER meshContainer;
	D3DXFRAME frame1, frame2; // bones

	LPD3DXANIMATIONCONTROLLER lpAnimationController;
private:
	LPD3DXKEYFRAMEDANIMATIONSET lpDefaultAnimationSet;


	D3DLIGHT9 defaultLight, pointLight;
	D3DMATERIAL9 defaultMaterial;
	D3DXMATERIAL rgbMaterial[3]; // red, green, blue with no texture
	

	D3DXMATRIX modelArcBallRotation, modelArcBallRotationLast;
	D3DXVECTOR3 cameraVector;
	BOOL cameraBounceDirection;
	BOOL leftPattern, rightPattern;

	LPDIRECT3DVERTEXBUFFER9 lpTestVB, lpTestVB2, lpBoxVB, lpSkinnedVB;
	void* pVBVertices;
	static const DWORD MY_COLOR_VERTEX_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	//static const DWORD MY_COLOR_VERTEX_NORMAL_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	BOOL rendering;
	BOOL closeNow;

	ModelReader mr1, mr2, mr3;
	ModelReader mrHouse;
	ModelReader mrMan;
	ModelReader gamebryo, middlesnake;
	ModelReader highpoly;
	ModelReader mrRocky;
	ModelReader mrDungeon;		// Dungeon(level) test
	D3DXVECTOR3 dungeonTranslation;

	ModelReader* pDrawingModel; // indicates drawing model at editor
	

	DWORD drawCount; // count for Draw() function call
	DWORD curFrameIndex; // current animation keyframe index (editor)

	int totalLightCount;
	BOOL isOkayToDestruct; // resolves multithread issue

	ARN_CAMERA mainCamera;

	

	std::list<RenderLayer*> m_renderLayers;

public:
	VideoMan();
	~VideoMan();

	HRESULT InitWindow(TCHAR* szClassName, WNDPROC wndProc, int width, int height);
	HRESULT InitD3D(BOOL isMultithreaded = FALSE);
	HRESULT InitTestVertexBuffer();
	HRESULT InitBoxVertexBuffer();
	HRESULT InitSkinnedMeshVertexBuffer();
	HRESULT InitTexture();
	HRESULT InitShader();
	HRESULT InitAnimationController(); // Global animation controller
	HRESULT InitCustomMesh();
	HRESULT InitMainCamera();
	HRESULT InitLight();
	HRESULT InitMaterial();
	HRESULT InitFont();

	HRESULT InitModels();
	
	const D3DXMATRIX* getArcballResult() const { return &modelArcBallRotation; }
	D3DMATERIAL9* getDefaultMaterial() { return &defaultMaterial; }

	HRESULT Show();
	HRESULT StartMainLoop();
	BOOL PauseMainLoop();
	BOOL ResumeMainLoop();
	int Draw();
	HRESULT RenderModel(const ModelReader* pMR, const D3DXMATRIX* worldTransformMatrix = 0);
	HRESULT RenderModel1(const ModelReader* pMR, const D3DXMATRIX* worldTransformMatrix = 0);
	HRESULT RenderModel2(const ModelReader *pMR, const D3DXMATRIX* worldTransformMatrix = 0);
	HRESULT TurnModelLightOn(const ModelReader *pMR, D3DXMATRIX* worldTransformMatrix = 0);
	void Close();

	void MoveMainCameraEye(float dx, float dy, float dz);
	void ChangeInTestVB(D3DCOLOR color);
	void ToggleLeftPattern();
	void ToggleRightPattern();

	HWND GetWindowHandle();
	LPDIRECT3DDEVICE9 GetDev();
	void SetDev(LPDIRECT3DDEVICE9 dev); // Aran Internal Use Only


	HRESULT SetCamera(float x, float y, float z);
	HRESULT SetCamera( ARN_NDD_CAMERA_CHUNK* pCamChunk );
	HRESULT SetCamera(ArnCamera& arnCam);

	void AttachInputMan(InputMan* inputMan);

	InputMan* GetInputMan();

	void SetWindowSize(int w, int h);
	void SetHwnd(HWND hWnd);

	void DrawAtEditor(BOOL isReady, BOOL isRunning);
	HRESULT InitModelsAtEditor();

	LPD3DXANIMATIONCONTROLLER GetAnimationController(); // global animation controller
	void SetDrawingModelAtEditor(ModelReader* pMR);
	HRESULT SetCurrentFrameIndex(int idx);

	virtual void ScrollBy( D3DXVECTOR3* dScroll );
	//LRESULT CALLBACK LoadingDialogProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);


	size_t registerRenderLayer(RenderLayer* pRL);
	BOOL unregisterRenderLayerAt( unsigned int ui );
	size_t unregisterAllRenderLayers();
	RenderLayer* getRenderLayerAt(unsigned int ui);

	const ARN_CAMERA* getMainCamera() const { return &mainCamera; }
	const void getScreenInfo(int& width, int& height) { width = screenWidth; height = screenHeight; }

	const D3DXMATRIX* getModelArcBallRotation() const { return &modelArcBallRotation; }
	const LPD3DXFONT getDefaultFont() const { return lpFont; }


	void setWorldViewProjection( const D3DXMATRIX& matWorld, const D3DXMATRIX& matView, const D3DXMATRIX& matProj );

	void renderMeshesOnly(ArnNode* node, const D3DXMATRIX& globalXform = DX_CONSTS::D3DXMAT_IDENTITY);
	void renderSingleMesh(ArnMesh* mesh, const D3DXMATRIX& globalXform = DX_CONSTS::D3DXMAT_IDENTITY);
	float getFPS() const { return m_fFPS; }

private:
	double m_prevLastTime;
	DWORD m_prevUpdateFrames;
	float m_fFPS;
	CPreciseTimer m_preciseTimer;

	std::vector<float> testFloatArray;
};




void SetSimpleColoredMaterial(D3DMATERIAL9* material, D3DXCOLOR color);




