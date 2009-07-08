#pragma once

class Trigger;
class Action;
class Incident;
class UnitBase;
class Dialog;
class SkillObject;
class Enemy;

typedef std::list<Trigger*>			TriggerList;
typedef std::list<Action*>			ActionList;
typedef std::list<std::string>		ArnStringList;
typedef std::vector<std::string>	StringVector;
typedef std::list<const char*>		ConstCharList;
typedef std::list<UnitBase*>		UnitSet;
typedef std::list<Dialog*>			DialogList;
typedef std::list<Incident*>		IncidentList;
typedef std::list<SkillObject*>		SkillObjectList;
typedef std::list<Enemy*>			EnemyList;
