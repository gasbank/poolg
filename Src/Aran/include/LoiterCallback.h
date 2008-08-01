#pragma once
#include "characteranimationcallback.h"

class LoiterCallback :
	public CharacterAnimationCallback
{
public:
	LoiterCallback(void);
	~LoiterCallback(void);

	virtual void DoCallbackFirstTimeOnly( void* pData /* in */, void* pResultData /* out */ );
	virtual void DoCallback( void* pData /* in */, void* pResultData /* out */ );

	virtual void DoUnregisterCallback();
};
