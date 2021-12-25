#include "DataFileManager.h"
#include "../Managers.h"
#include "RuntimeUtils/GenericEx/Common.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionData.h"

DataFileManager::DataFileManager()
	: DataFileManagerBase() { }

DataFileManager::~DataFileManager() { }

bool DataFileManager::Init()
{
	if(m_pDataFactory == nullptr)
		m_pDataFactory = CDataFileFactory::Instance();
	
	if(!Managers::IsInit())
		return false;

	LoadAllData();
	
	return true;
}

void DataFileManager::GetActionDatas(uint32 nDataID, TArray<UActionData*>& result)
{
	if (m_mapAllActionData.Contains(nDataID))
	{
		result = m_mapAllActionData[nDataID];
	}
	else
	{
		const CCharacterData* cData = GetData<CCharacterData>(nDataID);
		if (!cData)
			return;

		FString strDir = FPaths::Combine(cData->RefPath, TEXT("ActionDatas"));

		LoadUAssetAtPath(strDir, result);

		for (int i = 0 ; i < result.Num(); ++i)
			result[i]->AfterLoad();
			
		m_mapAllActionData.Add(nDataID, result);
	}
}
