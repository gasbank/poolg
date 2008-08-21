#pragma once
#include "Unit.h"

class BattleState;

class SkillObject
{
public:
	virtual ~SkillObject();
	virtual HRESULT frameRender ();
	virtual bool frameMove ( float fElapsedTime );

	static SkillObject* createSOnormalAttack (Character* user, Character* target, Unit* effectObject);
	static SkillObject* createSOheal (Character* user, Unit* effectObject);
	static SkillObject* createSOmeditation (Character* usedr, Unit* effectObject);
	static SkillObject* createSOcsBurn (Character* target, Unit* effectObject);
	static SkillObject* createSOmtBullet (Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber);


protected:
	BattleState* getBattleState();
	Unit* m_effectObject;
};

class SOnormalAttack : public SkillObject
{
public:
	~SOnormalAttack();
	virtual bool frameMove ( float fElapsedTime );

	friend class SkillObject;

protected:
	Character* m_user;
	Character* m_target;

private:
	SOnormalAttack (Character* user, Character* target, Unit* effectObject);
};

class SOheal : public SkillObject
{
public:
	~SOheal();
	virtual bool frameMove ( float fElapsedTime );

	friend class SkillObject;

protected:
	Character* m_user;

private:
	SOheal (Character* user, Unit* effectObject);
};

class SOmeditation : public SkillObject
{
public:
	~SOmeditation();
	virtual bool frameMove ( float fElapsedTime );

	friend class SkillObject;

protected:
	Character* m_user;

private:
	SOmeditation (Character* user, Unit* effectObject);
};

class SOcsBurn : public SkillObject
{
public:
	~SOcsBurn();
	virtual bool frameMove ( float fElapsedTime );

	friend class SkillObject;

protected:
	Character* m_target;

private:
	SOcsBurn (Character* target, Unit* effectObject);
};


//MultiThreadBullet
class SOmtBullet : public SkillObject
{
public:
	~SOmtBullet();
	virtual bool frameMove ( float fElapsedTime );

	friend class SkillObject;

protected:
	Character* m_user;
	Character* m_target;
	int m_leftNumber;
	int m_maxNumber;
	float m_elapsedTime;

private:
	SOmtBullet (Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber);
};
