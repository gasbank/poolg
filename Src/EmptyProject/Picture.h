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

	void init(const TCHAR* imgFileName, LPDIRECT3DDEVICE9 d3dDev, UINT segments = 1);
	void release();
	void draw();
	const D3DXMATRIX* getLocalXform() const { return &m_localXform; }
	void setSize(float width, float height) { m_width = width; m_height = height; }
	const D3DXVECTOR3* getPos() const { return &m_vPos; }
	void setPos(const D3DXVECTOR3& val) { m_vPos = val; }

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void frameMove(float fElapsedTime);

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
};
