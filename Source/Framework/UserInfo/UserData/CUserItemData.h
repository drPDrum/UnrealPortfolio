#pragma once
#include "DataLoadLib/DataFile/DataStructure/CItemData.h"

class CUserItemData
{
private:
	uint64						m_lKey;
	const CItemData*			m_pData;
	uint32						m_nStackCount;

public:
	CUserItemData(uint64 lKey, int32 nDataID, uint32 nStackCount = 1);
	//TODO : Make by protocol info
	~CUserItemData();

	uint64 GetKey() const;
	const CItemData* GetData() const;	
	uint32 GetStackCount() const;

	void RemoveCount();
};
