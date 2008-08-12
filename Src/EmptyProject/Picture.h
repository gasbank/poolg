#pragma once

#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01

struct Vertex
{
	float x, y, z;
	float u, v;

	Vertex(float _x, float _y, float _z, float _u, float _v)
	{
		x = _x;
		y = _y;
		z = _z;
		u = _u;
		v = _v;
	}
};
struct VertexRhw
{
	float x, y, z, rhw;
	float u, v;

	VertexRhw(float _x, float _y, float _z, float _rhw, float _u, float _v)
	{
		x = _x;
		y = _y;
		z = _z;
		rhw = _rhw;
		u = _u;
		v = _v;
	}
};

enum PictureInput
{
	PIC_MOVE_UP = 0,
	PIC_MOVE_DOWN,
	PIC_MOVE_LEFT,
	PIC_MOVE_RIGHT,
	PIC_MAX_KEYS,
	PIC_UNKNOWN = 0xFF
};

class Picture
{
public:
	Picture(void);
	virtual ~Picture(void);

	void init(const TCHAR* imgFileName, LPDIRECT3DDEVICE9 d3dDev, UINT segments = 1, float x = 0, float y = 0);
	HRESULT initRhw(const TCHAR* imgFileName, LPDIRECT3DDEVICE9 d3dDev, float x = 0, float y = 0);
	void release();
	
	const D3DXMATRIX* getLocalXform() const { return &m_localXform; }
	void setLocalXform(const D3DXMATRIX* mWorld) { m_localXform = *mWorld; }
	void setSize(float width, float height)
	{
		m_width = width; m_height = height;
		D3DXMATRIX rotMat, scaleMat, transMat;
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixScaling(&scaleMat, m_width, m_height, 1.0f);
		D3DXMatrixTranslation(&transMat, m_vPos.x, m_vPos.y, 10.0f);
		
		m_localXform = rotMat * scaleMat * transMat;
	}
	void setSizeToTexture();
	const D3DXVECTOR3* getPos() const { return &m_vPos; }
	void setPos(const D3DXVECTOR3& val) { m_vPos = val; }
	void setPos(float x, float y, float z) { m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;}

	HRESULT draw(bool textured = true);
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void frameMove(float fElapsedTime);

protected:
	virtual PictureInput mapKey( UINT nKey );
//[재우]이 안의 변수를 상속받은 객체에서 써야 하기에 protected로 하였습니다.
//private:
	bool IsKeyDown( BYTE key ) const { return( (key & KEY_IS_DOWN_MASK) == KEY_IS_DOWN_MASK ); }
	bool WasKeyDown( BYTE key ) const { return( (key & KEY_WAS_DOWN_MASK) == KEY_WAS_DOWN_MASK ); }
	
	D3DXMATRIX m_localXform;
	LPDIRECT3DDEVICE9 m_d3dDev;
	LPD3DXMESH m_d3dxMesh;
	LPDIRECT3DTEXTURE9 m_d3dTex;
	BYTE m_aKeys[PIC_MAX_KEYS];
	UINT m_cKeysDown;            // Number of camera keys that are down.
	D3DXVECTOR3 m_vKeyboardDirection;
	D3DXVECTOR3 m_vVelocity;
	D3DXVECTOR3 m_vPos;
	
	D3DXMATRIX m_mWorld;
	float m_width, m_height;

	// for RHW vertex
	LPDIRECT3DVERTEXBUFFER9 m_d3dVB;

	float m_texWidth, m_texHeight;

	bool m_bInit;
};
