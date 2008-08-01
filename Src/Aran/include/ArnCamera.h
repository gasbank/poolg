#pragma once
#include "ArnXformable.h"

struct NodeCamera1;
struct NodeCamera2;
struct NodeBase;

class ArnCamera : public ArnXformable
{
public:
	ArnCamera();
	~ArnCamera(void);

	static ArnNode*		createFrom(const NodeBase* nodeBase);
	const ARN_NDD_CAMERA_CHUNK& getCameraData() const { return m_cameraData; }

	// *** INTERNAL USE ONLY START ***
	virtual void		interconnect(ArnNode* sceneRoot);
	// *** INTERNAL USE ONLY END ***
private:
	void				buildFrom(const NodeCamera1* nc);
	void				buildFrom(const NodeCamera2* nc);

	ARN_NDD_CAMERA_CHUNK m_cameraData;

};
