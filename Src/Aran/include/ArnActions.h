#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeAction1;

class ArnActions : public ArnNode
{
public:
	ArnActions(void);
	~ArnActions(void);

	static ArnActions*				createFrom(const NodeBase* nodeBase);

	// *** INTERNAL USE ONLY START ***
	virtual void				interconnect(ArnNode* sceneRoot);
	// *** INTERNAL USE ONLY END ***

private:
	void						buildFrom(const NodeAction1* ni);

	struct ActionsData
	{
		STRING actionName;
		std::vector<std::pair<STRING, STRING> > channels;
	};
	std::vector<ActionsData> m_data;
};
