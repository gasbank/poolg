#pragma once

struct ArnFileData;
class ArnNode;
class ArnHierarchy;
class ArnBone;

class ArnSceneGraph
{
public:
	ArnSceneGraph(const ArnFileData& afd);
	~ArnSceneGraph(void);

	static ArnSceneGraph*	createFrom(const ArnFileData& afd);
	ArnNode*				getSceneRoot() const { return m_sceneRoot; }

private:
	void					postprocessingARN20();
	void					buildBoneHierarchy( ArnHierarchy* hierNode, ArnNode* skelNode, ArnBone* parentBoneNode );
	const ArnFileData&		m_afd;
	EXPORT_VERSION			m_exportVersion;
	ArnNode*				m_sceneRoot;
};
