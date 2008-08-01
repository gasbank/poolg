#pragma once
#include "characteranimationcallback.h"

class UndefinedCallback :
	public CharacterAnimationCallback
{
public:
	UndefinedCallback(void);
	~UndefinedCallback(void);

	virtual void DoCallbackFirstTimeOnly( void* pData /* in */, void* pResultData /* out */ );
	virtual void DoCallback( void* pData /* in */, void* pResultData /* out */ );

	virtual void DoUnregisterCallback();
};
