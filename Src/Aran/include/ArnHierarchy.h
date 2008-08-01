#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeHierarchy1;

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

	std::vector<MyFrameData>	m_data;
};
