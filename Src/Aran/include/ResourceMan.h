#pragma once

#include <map>
#include "Singleton.h"

class ModelReader;

class ResourceMan : public Singleton<ResourceMan>
{
public:
	ResourceMan(void);
	~ResourceMan(void);


	enum MODELID
	{
		POOLC,
		MAN,
		DUN,
		BIGHOUSE,
		MOMA,
		

		BOX0,
		BOX1,
		BOX2,
		BOX3,
		BOXSKIN,

		MODEL_COUNT,
	};
	HRESULT registerModel( MODELID id, const TCHAR* modelFileName );
	HRESULT unregisterModel( MODELID id );
	int initializeAll();
	HRESULT unregisterAllModels();
	const ModelReader* getModel( MODELID id ) const;

private:
	typedef std::map<MODELID, ModelReader*> ModelMap;
	ModelMap m_models;
};
