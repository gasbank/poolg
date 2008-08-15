#pragma once

namespace Aran
{
	class Character;
}

class CharacterAnimationCallback
{
public:
	CharacterAnimationCallback(void);
	~CharacterAnimationCallback(void);

	virtual void DoCallbackFirstTimeOnly( void* pData /* in */, void* pResultData /* out */ );
	virtual void DoCallback( void* pData /* in */, void* pResultData /* out */ );

	virtual void DoUnregisterCallback();
	void AttachCharacter( Aran::Character* pCharacter );
	Aran::Character* GetCharacter();

private:
	Aran::Character* pCharacter;
};
