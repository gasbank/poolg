#pragma once

class UnitBase;

class CommonWorldInterface
{
public:
	virtual bool					detachUnit( UnitBase* pUnit ) = 0;
};
