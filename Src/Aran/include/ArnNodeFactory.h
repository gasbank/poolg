#pragma once

class ArnNode;
struct NodeBase;

class ArnNodeFactory
{
public:
	static ArnNode* createFromNodeBase(const NodeBase* nodeBase);
private:
	ArnNodeFactory(void);
	~ArnNodeFactory(void);
};
