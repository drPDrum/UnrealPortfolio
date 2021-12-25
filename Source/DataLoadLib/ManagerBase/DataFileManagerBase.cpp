#include "DataFileManagerBase.h"
#include "DataLoadLib/DataLoadLibInfo.h"
#include "RuntimeUtils/GenericEx/Common.h"

DataFileManagerBase::DataFileManagerBase()
{
	m_pDataFactory = CDataFileFactory::Instance();
	ConstData = nullptr;
}

DataFileManagerBase::~DataFileManagerBase()
{
	if(m_pDataFactory != nullptr)
	{
		m_pDataFactory->ClearAllDataFiles();
		m_pDataFactory = nullptr;
	}

	if(ConstData != nullptr)
		ConstData = nullptr;		
}

void DataFileManagerBase::LoadAllData()
{
	if(m_pDataFactory == nullptr)
		m_pDataFactory = CDataFileFactory::Instance();
	
	// //TODO : 경로 관련 리펙토링 고민
	TArray<UBinaryDataSource*> arrAssets;

	LoadUAssetAtPath<UBinaryDataSource>(DataLoadLibInfo::PATH_CONTENTS_BINARY_DATA, arrAssets);
	
	for(auto& uAsset : arrAssets)
	{
		if(!uAsset)
			continue;;
	
		EDataFileEnum eData = static_cast<EDataFileEnum>(uAsset->DataFileType);
	
		if(m_mapAllData.Contains(eData))
			continue;
	
		auto map = m_pDataFactory->CreateDataFile(eData, uAsset);
		if(map != nullptr)
			m_mapAllData.Add(eData, map);
	}
	
	ConstData = m_pDataFactory->GetConstData();
}
