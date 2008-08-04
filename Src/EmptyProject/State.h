#pragma once

class State
{
public:
	State(void);
	virtual ~State(void);

	virtual void enter() = 0;
	virtual void leave() = 0;

	virtual void frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime) = 0;
	virtual void frameMove(double fTime, float fElapsedTime) = 0;

	virtual void release() = 0;
};
