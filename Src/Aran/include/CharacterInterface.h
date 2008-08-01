#pragma once

struct D3DXMATRIX;

enum CharacterAnimationState;

class CharacterInterface
{
public:
	enum CharacterAnimationState {
		CAS_UNDEFINED,
		CAS_INVISIBLE,
		CAS_LOITER,
		CAS_WALKING,
		CAS_RUNNING,
		CAS_SITDOWN,
		CAS_DEAD,
		/* should not be changed below */
		CAS_SIZE,
		CAS_FORCE_DWORD = 0x7fffffff,
	};

	CharacterInterface(void);
	~CharacterInterface(void);

	virtual void ChangeTranslationToLookAtDirection( float amount ) = 0;
	virtual void ChangeTranslation( float dx, float dy, float dz ) = 0;		// position
	virtual void ChangeOrientation( float dx, float dy, float dz ) = 0;		// rotation
	virtual const D3DXMATRIX* GetFinalTransform() const = 0;
	
	virtual void SetCharacterAnimationState(CharacterAnimationState cas) = 0;
	virtual void SetCharacterAnimationStateNext(CharacterAnimationState cas) = 0;
	virtual CharacterAnimationState GetCharacterAnimationState() = 0;
	virtual CharacterAnimationState GetCharacterAnimationStateNext() = 0;


	
};
