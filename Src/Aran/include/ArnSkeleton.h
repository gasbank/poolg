#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeSkeleton1;
struct NodeSkeleton2;

class ArnSkeleton : public ArnNode
{
public:
	ArnSkeleton(void);
	~ArnSkeleton(void);

	static ArnNode*			createFrom(const NodeBase* nodeBase);
	const SkeletonData&		getSkeletonData() const { return m_data; }
private:
	void					buildFrom(const NodeSkeleton1* ns);
	void					buildFrom(const NodeSkeleton2* ns);
	
	SkeletonData			m_data;
};
