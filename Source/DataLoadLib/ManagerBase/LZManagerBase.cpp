#include "LZManagerBase.h"

#include "DataLoadLib/DataLoadLibInfo.h"
#include "DataLoadLib/DataFile/CDataFileFactory.h"
#include "RuntimeUtils/GenericEx/Common.h"

LZManagerBase::LZManagerBase()
{
	m_pDataFactory = CDataFileFactory::Instance();
	m_mapAllData = nullptr;// m_pDataFactory->GetLocalizationMap();
	
	m_eCurLanguage = ELanguage::Korean;	
}

LZManagerBase::~LZManagerBase()
{
	if(m_pDataFactory != nullptr)
	{
		m_pDataFactory->ClearAllDataFiles();
		m_pDataFactory = nullptr;
	}
}

void LZManagerBase::LoadAllData()
{
	if(m_pDataFactory == nullptr)
		m_pDataFactory = CDataFileFactory::Instance();

	//TODO : 경로 관련 리펙토링 고민
	TArray<UBinaryDataSource*> arrAssets;

	LoadUAssetAtPath<UBinaryDataSource>(DataLoadLibInfo::PATH_CONTENTS_BINARY_DATA_LZ, arrAssets);
	
	for(auto& uAsset : arrAssets)
	{
		if(!uAsset)
			continue;;

		m_pDataFactory->CreateLocalizationFile(uAsset);
	}

	m_mapAllData = m_pDataFactory->GetLocalizationMap();
}

void LZManagerBase::ChangeLanguage(ELanguage eLanguage)
{
	m_eCurLanguage = eLanguage;
}

const FString& LZManagerBase::Get(const FString& strKey, ELanguage eLanguage) const
{
	if(eLanguage == ELanguage::None)
		eLanguage = m_eCurLanguage;

	if(m_mapAllData->Contains(strKey))
		return m_mapAllData->FindChecked(strKey)->GetLanguage(eLanguage);

	//return CLocalizationData::KEY_EMPTY;
	return strKey;
}

const FString& LZManagerBase::operator[](const FString& strKey) const
{
	return Get(strKey, m_eCurLanguage);
}
