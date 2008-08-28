#pragma once

class WindowMover
{
public:
	void setPos (D3DXVECTOR3 onPos, D3DXVECTOR3 offPos);
	void onBox();
	void offBox();
	bool isOn() {return m_isOn;}
	bool isMoving () {return (m_movingDirect^m_isOn);}

	void setOff ();

	void frameMove( float fElapsedTime );

	D3DXVECTOR3 getPos();

private:

	D3DXVECTOR3 m_onPos;
	D3DXVECTOR3 m_offPos;

	D3DXVECTOR3 m_curPos;

	bool m_movingDirect;
	bool m_isOn;
};