// InputMan.h
// 2007 Geoyeob Kim

#pragma once


typedef int BOOL;

class CharacterInterface;
class DungeonInterface;

struct Point2Int 
{
	int x, y;
};

class InputMan
{
private:
	BOOL isClicked;
	BOOL isRClicked;
	BOOL isDragging;
	Point2Int mouseCurPos;
	Point2Int mouseDownPos;
	Point2Int mouseUpPos;

	CharacterInterface* pCharInterface;
	DungeonInterface* pDungeonInterface;

	/*IDirectInput8*						lpDInput8;
	IDirectInputDevice8*				lpDInputDevKeyboard;*/
	
public:
	InputMan(void);
	~InputMan(void);

	HRESULT Initialize( HINSTANCE hInst, HWND hwnd );
	HRESULT AcquireKeyboard();

	void SetClicked(BOOL b) { this->isClicked = b; }
	void SetRClicked(BOOL b) { this->isRClicked = b; }
	void SetDragging(BOOL b) { this->isDragging = b; }

	void SetMouseCurPos(Point2Int p) { this->mouseCurPos = p; }
	void SetMouseCurPos(int x, int y) { this->mouseCurPos.x = x; this->mouseCurPos.y = y; }
	void SetMouseDownPos(Point2Int p) { this->mouseDownPos = p; }
	void SetMouseDownPos(int x, int y) { this->mouseDownPos.x = x; this->mouseDownPos.y = y; }
	void SetMouseUpPos(Point2Int p) { this->mouseUpPos = p; }
	void SetMouseUpPos(int x, int y) { this->mouseUpPos.x = x; this->mouseUpPos.y = y; }
	

	BOOL IsClicked() { return this->isClicked; }
	BOOL IsRClicked() { return this->isRClicked; }
	BOOL IsDragging() { return this->isDragging; }

	Point2Int GetMouseCurPos() { return this->mouseCurPos; }
	Point2Int GetMouseDownPos() { return this->mouseDownPos; }
	Point2Int GetMouseUpPos() { return this->mouseUpPos; }


	// Player character
	void AttachCharacterInterface( CharacterInterface* pCharInterface ) { this->pCharInterface = pCharInterface; }

	// Player character control
	void WalkCharacterForward( float amount = 0.2f );
	void WalkCharacterBackward( float amount = 0.2f );
	void TurnCharacterLeft( float amount = D3DXToRadian( 5 ) );
	void TurnCharacterRight( float amount = D3DXToRadian( 5 ) );
	const D3DXMATRIX* GetFinalTransform() const;

	void StopCharacterWalking();

	// Dungeon
	void AttachDungeonInterface( DungeonInterface* pDungeonInterface ) { this->pDungeonInterface = pDungeonInterface; }
	void DungeonScrollBy( D3DXVECTOR3* pDScroll );

	HRESULT WINAPI ProcessKeyboardInput();
};
