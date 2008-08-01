#pragma once
#include "ArnObject.h"

class ArnNode : public ArnObject
{
public:
	typedef std::list<ArnNode*> ChildrenList;

							ArnNode(NODE_DATA_TYPE type);
	virtual					~ArnNode(void);

	ArnNode*				getParent() const { return m_parent; }
	const STRING&			getParentName() const { return m_parentName; }
	const char*				getName() const { return m_name.c_str(); }
	void					setName(const char* name) { m_name = name; }
	void					attachChild(ArnNode* child);
	void					detachChild(ArnNode* child);
	void					deleteAllChildren();
	ArnNode*				getLastNode();
	ArnNode*				getNodeByName(const STRING& name);
	ArnNode*				getNodeAt(unsigned int idx);
	unsigned int			getNodeCount() const { return m_children.size(); }
	ArnNode*				getSceneRoot() { if (m_parent) return m_parent->getSceneRoot(); else return this; }
	virtual void			update(double fTime, float fElapsedTime);
	// *** INTERNAL USE ONLY START ***
	virtual void			interconnect(ArnNode* sceneRoot);
	void					setParent(ArnNode* node) { m_parent = node; }
	void					detachParent() { if (m_parent) m_parent->detachChild(this); }
	// *** INTERNAL USE ONLY END ***
protected:
	void					setParentName(const char* name) { m_parentName = name; }

private:
	STRING					m_name;
	ArnNode*				m_parent;
	STRING					m_parentName;
	ChildrenList			m_children;
};
