#include "DataLoadLib/DataLoadLibInfo.h"
#include "DataLoadLib/ManagerBase/DataFileManagerBase.h"
#include "DataLoadLib/ManagerBase/LZManagerBase.h"
#include "RuntimeUtils/GenericEx/Common.h"
#include "Tests/AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckBinaryDataLoadTime, "DrumTest.BinaryData.TimeCheckData",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool CheckBinaryDataLoadTime::RunTest(const FString& Parameters)
{
	DataFileManagerBase* pMgr = new DataFileManagerBase();
	
	TestNotNull("DataFileManagerBase Not null", pMgr);

	const double dStartTime = FPlatformTime::Seconds();
	pMgr->LoadAllData();
	const double dEndTime = FPlatformTime::Seconds();

	AddEvent(FAutomationEvent(EAutomationEventType::Info, FString::Printf(TEXT("Check All Data Loadtime : '%lf' Sec"), dEndTime - dStartTime)));

	delete pMgr;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckBinaryLZDataLoadTime, "DrumTest.BinaryData.TimeCheckLZ",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool CheckBinaryLZDataLoadTime::RunTest(const FString& Parameters)
{
	LZManagerBase* pMgr = new LZManagerBase();
	
	TestNotNull("DataFileManagerBase Not null", pMgr);

	const double dStartTime = FPlatformTime::Seconds();
	pMgr->LoadAllData();
	const double dEndTime = FPlatformTime::Seconds();

	AddEvent(FAutomationEvent(EAutomationEventType::Info, FString::Printf(TEXT("Check All LZ Loadtime : '%lf' Sec"), dEndTime - dStartTime)));

	delete pMgr;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ValidateBinaryData, "DrumTest.BinaryData.ValidateData",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool ValidateBinaryData::RunTest(const FString& Parameters)
{
	TArray<UBinaryDataSource*> arrAssets;
	LoadUAssetAtPath<UBinaryDataSource>(DataLoadLibInfo::PATH_CONTENTS_BINARY_DATA, arrAssets);

	if(arrAssets.Num() == 0)
	{
		AddEvent(FAutomationEvent(EAutomationEventType::Info, FString::Printf(TEXT("No Asset in '%s'"), *DataLoadLibInfo::PATH_CONTENTS_BINARY_DATA)));
		return true;
	}

	TArray<int32> arrID;

	for(auto& pData : arrAssets)
	{
		arrID.Reset();

		for(int i = 0 ; i < pData->TableInfo.Num() ; ++i)
		{
			auto& rowData = pData->TableInfo[i];
			
			int32 nID = FCString::Atoi(*rowData.arrColData[0]);

			if(nID == 0)
			{
				AddError(FString::Printf(TEXT("Key ERROR '0' Line='%d' in '%s'"),
					i + 4, *pData->GetName()), 1);
				continue;
			}

			if(arrID.Contains(nID))
			{
				AddError(FString::Printf(TEXT("Key ERROR 'Duplicated' ID='%d' Line='%d' in '%s'"),
					nID, i + 4, *pData->GetName()), 1);
				continue;
			}

			arrID.Add(nID);
		}
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ValidateBinaryLZData, "DrumTest.BinaryData.ValidateLZ",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool ValidateBinaryLZData::RunTest(const FString& Parameters)
{
	TArray<UBinaryDataSource*> arrAssets;
	LoadUAssetAtPath<UBinaryDataSource>(DataLoadLibInfo::PATH_CONTENTS_BINARY_DATA_LZ, arrAssets);

	if(arrAssets.Num() == 0)
	{
		AddEvent(FAutomationEvent(EAutomationEventType::Info, FString::Printf(TEXT("No Asset in '%s'"), *DataLoadLibInfo::PATH_CONTENTS_BINARY_DATA_LZ)));		
		return true;
	}

	TArray<FString> arrID;

	for(auto& pData : arrAssets)
	{
		arrID.Reset();

		for(int i = 0 ; i < pData->TableInfo.Num() ; ++i)
		{
			auto& rowData = pData->TableInfo[i];

			FString strID = rowData.arrColData[0];

			if(arrID.Contains(strID))
			{ 
				AddError(FString::Printf(TEXT("Key ERROR 'Duplicated' KEY='%s' Line='%d' in '%s'"),
					*strID, i + 4, *pData->GetName()), 1);
				continue;
			}

			arrID.Add(strID);
		}
	}
	
	return true;
}