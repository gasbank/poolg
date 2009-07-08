#pragma once

class WindowMover
{
public:
	void setPos (ArnVec3 onPos, ArnVec3 offPos);
	void onBox();
	void offBox();
	bool isOn() {return m_isOn;}
	bool isMoving () {return (m_movingDirect^m_isOn);}

	void setOff ();

	void frameMove( float fElapsedTime );

	ArnVec3 getPos();

private:

	ArnVec3 m_onPos;
	ArnVec3 m_offPos;

	ArnVec3 m_curPos;

	bool m_movingDirect;
	bool m_isOn;
};