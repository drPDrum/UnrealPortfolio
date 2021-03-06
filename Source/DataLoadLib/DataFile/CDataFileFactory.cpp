// Generate By DataTool. 2021-12-25 오전 7:02:31
// Drum
#include "CDataFileFactory.h"

void CDataFileFactory::ClearAllDataFiles()
{
	if(m_pContainer == nullptr)
		return;

	if(m_pContainer->ConstData != nullptr)
	{
		delete m_pContainer->ConstData;
		m_pContainer->ConstData = nullptr;
	}

	ClearAllDataFiles(m_pContainer->MapCharacterData);
	ClearAllDataFiles(m_pContainer->MapItemData);
	ClearAllDataFiles(m_pContainer->MapHelpData);
	ClearAllDataFiles(m_pContainer->MapHelpCategoryData);
	ClearAllDataFiles(m_pContainer->MapLandData);
	ClearAllDataFiles(m_pContainer->MapStatData);

	ClearAllDataFiles(m_pContainer->MapLocalizationData);

	delete m_pContainer;
	m_pContainer = nullptr;
}

void* CDataFileFactory::CreateDataFile(EDataFileEnum eData, const UBinaryDataSource* uData)
{
	if(uData == nullptr)
		return nullptr;

	if(m_pContainer == nullptr)
		m_pContainer = new CDataFileContainer();

	switch (eData)
	{
	case EDataFileEnum::ConstData:
		if(m_pContainer->ConstData == nullptr)
			m_pContainer->ConstData = new CConstData();
		m_pContainer->ConstData->SetInfo(uData->TableInfo);
		return nullptr;
	case EDataFileEnum::CharacterData:
		CreateDataFile<CCharacterData>(uData, m_pContainer->MapCharacterData);
		return &m_pContainer->MapCharacterData;
	case EDataFileEnum::ItemData:
		CreateDataFile<CItemData>(uData, m_pContainer->MapItemData);
		return &m_pContainer->MapItemData;
	case EDataFileEnum::HelpData:
		CreateDataFile<CHelpData>(uData, m_pContainer->MapHelpData);
		return &m_pContainer->MapHelpData;
	case EDataFileEnum::HelpCategoryData:
		CreateDataFile<CHelpCategoryData>(uData, m_pContainer->MapHelpCategoryData);
		return &m_pContainer->MapHelpCategoryData;
	case EDataFileEnum::LandData:
		CreateDataFile<CLandData>(uData, m_pContainer->MapLandData);
		return &m_pContainer->MapLandData;
	case EDataFileEnum::StatData:
		CreateDataFile<CStatData>(uData, m_pContainer->MapStatData);
		return &m_pContainer->MapStatData;
	}

	return nullptr;
}

const CDataFileBase* CDataFileFactory::GetData(EDataFileEnum eData, int32 nKey) const
{
	if(m_pContainer == nullptr)
		return nullptr;

	switch (eData)
	{
	case EDataFileEnum::ConstData:
		return nullptr;
	case EDataFileEnum::CharacterData:
		return m_pContainer->MapCharacterData.Contains(nKey) ?
			m_pContainer->MapCharacterData[nKey] : nullptr;
	case EDataFileEnum::ItemData:
		return m_pContainer->MapItemData.Contains(nKey) ?
			m_pContainer->MapItemData[nKey] : nullptr;
	case EDataFileEnum::HelpData:
		return m_pContainer->MapHelpData.Contains(nKey) ?
			m_pContainer->MapHelpData[nKey] : nullptr;
	case EDataFileEnum::HelpCategoryData:
		return m_pContainer->MapHelpCategoryData.Contains(nKey) ?
			m_pContainer->MapHelpCategoryData[nKey] : nullptr;
	case EDataFileEnum::LandData:
		return m_pContainer->MapLandData.Contains(nKey) ?
			m_pContainer->MapLandData[nKey] : nullptr;
	case EDataFileEnum::StatData:
		return m_pContainer->MapStatData.Contains(nKey) ?
			m_pContainer->MapStatData[nKey] : nullptr;
	}

	return nullptr;
}

const CConstData* CDataFileFactory::GetConstData() const
{
	if(m_pContainer == nullptr)
		return nullptr;

	return m_pContainer->ConstData;	
}

void CDataFileFactory::CreateLocalizationFile(const UBinaryDataSource* uData)
{
	if(uData == nullptr)
		return;

	if(m_pContainer == nullptr)
		m_pContainer = new CDataFileContainer();

	for(auto& rowData : uData->TableInfo)
	{
		CLocalizationData* cData = new CLocalizationData();
		cData->SetInfo(rowData);

		if(m_pContainer->MapLocalizationData.Contains(cData->ID))
		{
			delete cData;
			continue;
		}

		m_pContainer->MapLocalizationData.Add(cData->ID, cData);
	}
}

const TMap<FString, CLocalizationData*>* CDataFileFactory::GetLocalizationMap() const
{
	if(m_pContainer == nullptr)
		return nullptr;

	return &m_pContainer->MapLocalizationData;
}

