#pragma once
#include "ArnNode.h"

struct NodeBase;
struct NodeBone1;
struct NodeBone2;

class ArnBone : public ArnNode
{
public:
	ArnBone(void);
	~ArnBone(void);

	static ArnNode*		createFrom(const NodeBase* nodeBase);
	void				setFrameData(const MyFrameData* frameData) { m_frameData = frameData; }
	const MyFrameData*	getFrameData() const { return m_frameData; }
	const BoneData&		getBoneData() const { return m_data; }

private:
	void				buildFrom(const NodeBone1* nb);
	void				buildFrom(const NodeBone2* nb);

	BoneData			m_data;
	const MyFrameData*	m_frameData;
};
