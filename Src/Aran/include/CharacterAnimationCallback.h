#pragma once

class Character;

class CharacterAnimationCallback
{
public:
	CharacterAnimationCallback(void);
	~CharacterAnimationCallback(void);

	virtual void DoCallbackFirstTimeOnly( void* pData /* in */, void* pResultData /* out */ );
	virtual void DoCallback( void* pData /* in */, void* pResultData /* out */ );

	virtual void DoUnregisterCallback();
	void AttachCharacter( Character* pCharacter );
	Character* GetCharacter();

private:
	Character* pCharacter;
};
