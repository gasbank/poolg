#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeHierarchy1;
struct NodeHierarchy2;

class ArnHierarchy : public ArnNode
{
public:
	ArnHierarchy(void);
	~ArnHierarchy(void);

	static ArnNode*				createFrom(const NodeBase* nodeBase);
	const MyFrameData&			getFrame(unsigned int idx) const;
	unsigned int				getFrameCount() const { return m_data.size(); }
private:
	void						buildFrom(const NodeHierarchy1* nh);
	void						buildFrom(const NodeHierarchy2* ns);

	std::vector<MyFrameData>	m_data;
	SkeletonData				m_data2;
};
