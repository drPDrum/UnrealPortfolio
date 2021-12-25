#pragma once
#include "CUserItemData.h"
#include "DataLoadLib/DataFile/DataStructure/CCharacterData.h"
#include "DataLoadLib/DataFile/DataStructure/CStatData.h"

class CUserCharData
{
private:
	uint64							m_lKey;
	const CCharacterData*			m_pData;
	const CStatData*				m_pStatData;

public:
	CUserCharData(uint64 lKey, int32 nDataID);
	//TODO : Make by protocol info
	~CUserCharData();

	void AddItem(CUserItemData* cItem);

	uint64 GetKey() const;
	const CCharacterData* GetData() const;
	const CStatData* GetStatData() const;
};
