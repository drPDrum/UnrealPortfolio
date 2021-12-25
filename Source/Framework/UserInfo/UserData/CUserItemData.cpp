#include "CUserItemData.h"

#include "Framework/Managers/Managers.h"

CUserItemData::CUserItemData(uint64 lKey, int32 nDataID, uint32 nStackCount)
{
	m_lKey = lKey;
	if(nDataID > 0)
		m_pData = Managers::Data->GetData<CItemData>(nDataID);
	else
		m_pData = nullptr;

	m_nStackCount = nStackCount;
}

CUserItemData::~CUserItemData()
{
	m_pData = nullptr;
}

uint64 CUserItemData::GetKey() const
{
	return m_lKey;
}

const CItemData* CUserItemData::GetData() const
{
	return m_pData;
}

uint32 CUserItemData::GetStackCount() const
{
	return m_nStackCount;
}

void CUserItemData::RemoveCount()
{
	if(m_nStackCount > 0)
		--m_nStackCount;
	else
	{
		LOG_ERROR(TEXT("Item Remove logic ERROR : Stack Count 0 : Key[%lld]"), m_lKey);
	}
}
