#include "CUserCharData.h"

#include "Framework/Managers/Managers.h"

CUserCharData::CUserCharData(uint64 lKey, int32 nDataID)
{
	m_lKey = lKey;
	if(nDataID > 0)
		m_pData = Managers::Data->GetData<CCharacterData>(nDataID);
	else
		m_pData = nullptr;
	
	if(m_pData)
		m_pStatData = Managers::Data->GetData<CStatData>(m_pData->DataIdStat);
	else
		m_pStatData = nullptr;
}

CUserCharData::~CUserCharData()
{
	m_pData = nullptr;
	m_pStatData = nullptr;	
}

uint64 CUserCharData::GetKey() const
{
	return m_lKey;
}

const CCharacterData* CUserCharData::GetData() const
{
	return m_pData;
}

const CStatData* CUserCharData::GetStatData() const
{
	return m_pStatData;
}
