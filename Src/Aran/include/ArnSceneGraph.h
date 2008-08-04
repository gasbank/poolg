#pragma once
#include "ArnObject.h"

struct ArnFileData;
class ArnNode;
class ArnHierarchy;
class ArnBone;

class ArnSceneGraph : public ArnObject
{
public:
	ArnSceneGraph(const ArnFileData& afd);
	~ArnSceneGraph(void);

	static ArnSceneGraph*	createFrom(const ArnFileData& afd);
	ArnNode*				getSceneRoot() const { return m_sceneRoot; }
	virtual const char*		getName() const { return "Scene Graph"; }
private:
	void					postprocessingARN20();
	void					buildBoneHierarchy( ArnHierarchy* hierNode, ArnNode* skelNode, ArnBone* parentBoneNode );
	const ArnFileData&		m_afd;
	EXPORT_VERSION			m_exportVersion;
	ArnNode*				m_sceneRoot;
};
