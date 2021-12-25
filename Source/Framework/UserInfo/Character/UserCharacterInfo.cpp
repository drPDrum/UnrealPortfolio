#include "UserCharacterInfo.h"
#include "../../CommonInfo.h"

uint64 UserCharacterInfo::LAST_KEY_CHARACTER = 0;
uint64 UserCharacterInfo::LAST_KEY_ITEM = 0;     

UserCharacterInfo::UserCharacterInfo()
{
	m_pCurCharacter = nullptr;
}

UserCharacterInfo::~UserCharacterInfo()
{
	if(m_pCurCharacter)
	{
		delete m_pCurCharacter;
		m_pCurCharacter = nullptr;
	}

	for (auto& tuple : m_mapInven)
	{
		if(tuple.Value)
		{
			delete tuple.Value;
			tuple.Value = nullptr;
		}		
	}

	m_mapInven.Empty();
}

bool UserCharacterInfo::Init()
{
	SetDummyCharacter();
	SetDummyInventory();

	return true;
}

CUserCharData* UserCharacterInfo::GetCurUserCharacter() const
{
	return m_pCurCharacter;
}

CUserItemData* UserCharacterInfo::GetItem(uint64 lKey) const
{
	if(m_mapInven.Contains(lKey))
		return m_mapInven[lKey];

	return nullptr;
}

TMap<uint64, CUserItemData*> UserCharacterInfo::GetInventory() const
{
	return m_mapInven;
}

CUserCharData* UserCharacterInfo::MakeNewCharacter(int32 nDataID)
{
	auto pChar = new CUserCharData(UserCharacterInfo::GetNewCharacterKey(), nDataID);

	if(m_mapCharacters.Contains(pChar->GetKey()))
	{
		LOG_ERROR(TEXT("Generate Character Key ERROR : Key[%lld] DataID[%d]"), pChar->GetKey(), nDataID);
		delete pChar;
		return nullptr;
	}

	m_mapCharacters.Add(pChar->GetKey(), pChar);
	return pChar;
}

CUserItemData* UserCharacterInfo::MakeNewItem(int32 nDataID, int nStackCount)
{
	auto pItem = new CUserItemData(UserCharacterInfo::GetNewItemKey(), nDataID);

	if(m_mapInven.Contains(pItem->GetKey()))
	{
		LOG_ERROR(TEXT("Generate Item Key ERROR : Key[%lld] DataID[%d]"), pItem->GetKey(), nDataID);
		delete pItem;
		return nullptr;
	}

	m_mapInven.Add(pItem->GetKey(), pItem);
	return pItem;
}

void UserCharacterInfo::SetDummyCharacter()
{
	if(m_pCurCharacter)
	{
		if(m_mapCharacters.Contains(m_pCurCharacter->GetKey()))
			m_mapCharacters.Remove(m_pCurCharacter->GetKey());
		
		delete m_pCurCharacter;
		m_pCurCharacter = nullptr;
	}

	m_pCurCharacter = MakeNewCharacter(1);
}

void UserCharacterInfo::SetDummyInventory()
{
	if(m_mapInven.Num() > 0)
	{
		for (auto& tuple : m_mapInven)
		{
			if(tuple.Value)
			{
				delete tuple.Value;
				tuple.Value = nullptr;
			}
		}

		m_mapInven.Empty();
	}

	MakeNewItem(1);
	MakeNewItem(2, 2);
	MakeNewItem(3, 3);
	MakeNewItem(4, 4);
	MakeNewItem(5, 5);

	MakeNewItem(1, 6);
	MakeNewItem(2, 7);
	MakeNewItem(3, 8);
	MakeNewItem(4, 9);
	MakeNewItem(5, 10);

	MakeNewItem(1, 11);
	MakeNewItem(2, 12);
	MakeNewItem(3, 13);
	MakeNewItem(4, 14);
	MakeNewItem(5, 15);
}

uint64 UserCharacterInfo::GetLastCharacterKey()
{
	return LAST_KEY_CHARACTER;
}

uint64 UserCharacterInfo::GetNewCharacterKey()
{
	return ++LAST_KEY_CHARACTER;
}

uint64 UserCharacterInfo::GetLastItemKey()
{
	return LAST_KEY_ITEM;
}

uint64 UserCharacterInfo::GetNewItemKey()
{
	return ++LAST_KEY_ITEM;
}
