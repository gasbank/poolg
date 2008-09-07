#pragma once

class Trigger;
class Action;
class Incident;
class Unit;
class Dialog;
class SkillObject;

typedef std::list<Trigger*>			TriggerList;
typedef std::list<Action*>			ActionList;
typedef std::list<std::string>		StringList;
typedef std::vector<std::string>	StringVector;
typedef std::list<const char*>		ConstCharList;
typedef std::set<Unit*>				UnitSet;
typedef std::list<Dialog*>			DialogList;
typedef std::list<Incident*>		IncidentList;
typedef std::list<SkillObject*>		SkillObjectList;

