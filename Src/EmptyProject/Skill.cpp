#include "EmptyProjectPCH.h"
#include "Skill.h"
#include "DynamicMotion.h"
#include "SkillObject.h"

using std::wstring;

SkillSet::SkillSet()
{
	m_skillLocation = SL_FIRST;

	for (int i = 0; i < 5; i++)
		m_skillSet[i] = NULL;
}

SkillSet::~SkillSet()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_skillSet[i] != NULL)
			delete m_skillSet[i];
	}
}



void SkillSet::setSkill (SkillLocation skillLocation, Skill* skill)
{
	if (m_skillSet[(int)skillLocation] != (NULL))
		delete m_skillSet[(int)skillLocation];
	m_skillSet[(int)skillLocation] = skill;
}

void SkillSet::deleteSkill (SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] != (NULL))
		delete m_skillSet[(int)skillLocation];
	m_skillSet[(int)skillLocation] = NULL;
}

wstring SkillSet::getSkillName (SkillLocation skillLocation)
{
	/*����� ��� NULL ��ȯ*/
	if (m_skillSet[(int)skillLocation] == NULL)
		return L"-";
	return m_skillSet[(int)skillLocation]->m_skillName;
}

bool SkillSet::useSkill ()
{
	if (m_skillSet[(int)this->m_skillLocation] == NULL)
		return false;

	m_skillSet[(int)this->m_skillLocation]->useSkill( m_battleState );
	
	return true;
}

void SkillSet::moveSkillLocation (char mode)
{
	switch (mode)
	{
	/*initialize*/
	case 'i':
		m_skillLocation = SL_FIRST;
		break;
	/*up*/
	case 'u':
		switch (m_skillLocation)
		{
		case SL_FIRST:
			m_skillLocation = SL_FIFTH;
			break;
		case SL_SECOND:
			m_skillLocation = SL_FIRST;
			break;
		case SL_THIRD:
			m_skillLocation = SL_SECOND;
			break;
		case SL_FOURTH:
			m_skillLocation = SL_THIRD;
			break;
		case SL_FIFTH:
			m_skillLocation = SL_FOURTH;
			break;
		}
		break;
	/*down*/
	case 'd':
		switch (m_skillLocation)
		{
		case SL_FIRST:
			m_skillLocation = SL_SECOND;
			break;
		case SL_SECOND:
			m_skillLocation = SL_THIRD;
			break;
		case SL_THIRD:
			m_skillLocation = SL_FOURTH;
			break;
		case SL_FOURTH:
			m_skillLocation = SL_FIFTH;
			break;
		case SL_FIFTH:
			m_skillLocation = SL_FIRST;
			break;
		}
		break;
	}
}

void SkillSet::setCharacter (Character* hero, Character* enemy)
{
	for (int i = 0; i < 5; i++)
	{
		if (m_skillSet[i] != NULL)
			m_skillSet[i]->setUser (hero, enemy);
	}
}


wstring SkillSet::getDescription (SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return L"��ų�� �����ϴ�.";
	return m_skillSet[(int)skillLocation]->getDescription();
}


/*
Skill ���ϴ� ����
�� ���� to TT_NATURAL
��ų ���ϴ� ��Ʈ
AttackObject������� ���� ��ġ���� or �ڱ��ڽ� ȿ��(buff). (���� or �ڰ�)
������ ��� ���� object�� ������, �ڰ��� ��� 
*/


void Skill::useSkill (BattleState *bs)
{
	int curCs = m_hero->getCurCs();

	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);

}

void NormalAttack::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();

	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("�Ϲ� ������ ����Ͽ����ϴ�.");
	//m_hero->doNormalAttack(0, m_enemy);

	D3DXVECTOR3 fireDir = m_enemy->getPos() - m_hero->getPos();
	float dist = D3DXVec3Length( &fireDir );

	D3DXVec3Normalize( &fireDir, &fireDir );



	LPD3DXMESH mesh;
	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
	Unit* missile = Unit::createUnit(mesh, 0, 0, 0);

	missile->setDynamicMotion(DynamicMotion::createDMfireUniformly
		(missile, m_hero->getPos(), fireDir, dist, 40.0f ));

	SkillObject* so = SkillObject::createSOnormalAttack( m_hero, m_enemy, missile);

	m_hero->pushSkillObject( so );

	//m_skillObjectList.push_back(so);

}

void Heal::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);


	bs->pushBattleLog("������ ����Ͽ����ϴ�.");

	LPD3DXMESH mesh;
	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0);
	Unit* healBall = Unit::createUnit(mesh, 0, 0, 0);

	healBall->setDynamicMotion(DynamicMotion::createDMspinAround
		(healBall, m_hero->getPos(), 5, 0.03f, 10));

	SkillObject* so = SkillObject::createSOheal(m_hero, healBall);
	m_hero->pushSkillObject( so );

}

void Meditation::useSkill(BattleState *bs)
{
	bs->pushBattleLog("�޵����̼��� ����Ͽ����ϴ�.");
	LPD3DXMESH mesh;
	D3DXCreateSphere( GetG().m_dev, 0.7f, 16, 16, &mesh, 0);
	Unit* healBall = Unit::createUnit(mesh, 0, 0, 0);

	healBall->setDynamicMotion(DynamicMotion::createDMspinAround
		(healBall, m_hero->getPos(), 5, 0.03f, 2));
    
	SkillObject* so = SkillObject::createSOmeditation( m_hero, healBall );
	m_hero->pushSkillObject ( so );
}

void MultiThread::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}
	
	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("��Ƽ�����带 ����Ͽ����ϴ�.");
	//m_hero->doMultiThread(3, m_enemy);

	D3DXVECTOR3 fireDir = m_enemy->getPos() - m_hero->getPos();
	float dist = D3DXVec3Length( &fireDir );

	D3DXVec3Normalize( &fireDir, &fireDir );

	int frequency = m_hero->getStat().sense / 4;

	for (int i=frequency-1; i >= 0; i--)
	{
		LPD3DXMESH mesh;
		D3DXCreateBox( GetG().m_dev, 0.5f, 8, 0.5f, &mesh, 0);
		Unit* effectObj = Unit::createUnit(mesh, 0, 0, 0);

		effectObj->setDynamicMotion(DynamicMotion::createDMfireUniformly
		(effectObj, m_hero->getPos(), fireDir, dist, 4.0f ));

		SkillObject* so = SkillObject::createSOmtBullet( m_hero, m_enemy, effectObj, i, frequency );
		m_hero->pushSkillObject( so );
	}

}

void Goto::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}
	
	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("�ݱ��� Goto�� ����Ͽ����ϴ�.");
	//m_hero->doGoto (m_enemy);

	LPD3DXMESH mesh;
	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0);
	Unit* effectObj = Unit::createUnit(mesh, 0, 0, 0);

	effectObj->setDynamicMotion(DynamicMotion::createDMpuff
		(effectObj, m_enemy->getPos(), 25, 0.2f));

	SkillObject* so = SkillObject::createSOgoto(m_hero, m_enemy, effectObj);
	m_hero->pushSkillObject( so );
}
