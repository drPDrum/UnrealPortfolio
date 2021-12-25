#pragma once
#include "Framework/UserInfo/IUserInfo.h"
#include "Framework/UserInfo/UserData/CUserCharData.h"

class UserCharacterInfo : public IUserInfo
{
private:
	CUserCharData*					m_pCurCharacter;
	TMap<uint64, CUserCharData*>	m_mapCharacters;
	TMap<uint64, CUserItemData*>	m_mapInven;

public:
	UserCharacterInfo();
	virtual ~UserCharacterInfo() override;

	virtual bool Init() override;

	CUserCharData* GetCurUserCharacter() const;
	CUserItemData* GetItem(uint64 lKey) const;
	TMap<uint64, CUserItemData*> GetInventory() const;

	CUserCharData* MakeNewCharacter(int32 nDataID);
	CUserItemData* MakeNewItem(int32 nDataID, int nStackCount = 1);

	//For test or single mode
	void SetDummyCharacter();
	void SetDummyInventory();

private:
	static uint64					LAST_KEY_CHARACTER;
	static uint64					LAST_KEY_ITEM;

public:
	static uint64 GetLastCharacterKey();
	static uint64 GetNewCharacterKey();
	static uint64 GetLastItemKey();
	static uint64 GetNewItemKey();
};
