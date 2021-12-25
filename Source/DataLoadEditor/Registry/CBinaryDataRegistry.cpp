#include "CBinaryDataRegistry.h"
#include "AssetToolsModule.h"
#include "CDirectoryStatVisitor.h"
#include "DirectoryWatcherModule.h"
#include "IDirectoryWatcher.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "DataLoadLib/BinaryDataSource.h"
#include "../Factory/BinaryDataFactory.h"
#include "../DataLoadEditorInfo.h"

CBinaryDataRegistry::CBinaryDataRegistry(UBinaryDataFactory* pFactory)
{
	if (!IsRunningCommandlet() && GIsEditor)
	{
		m_pFactory = pFactory;

		// Set Directory watcher

		FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>("DirectoryWatcher");
		IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule.Get();

		if (DirectoryWatcher)
		{
			DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(
				FPaths::ProjectContentDir(),
				IDirectoryWatcher::FDirectoryChanged::CreateRaw(this, &CBinaryDataRegistry::OnDirectoryChanged),
				GameDirectoryWatcherHandle);
		}
	}
}

CBinaryDataRegistry::~CBinaryDataRegistry()
{
	if (!IsRunningCommandlet() && GIsEditor)
	{
		if (FModuleManager::Get().IsModuleLoaded("DirectoryWatcher"))
		{
			FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::GetModuleChecked<FDirectoryWatcherModule>("DirectoryWatcher");
			IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule.Get();

			if (DirectoryWatcher)
			{
				DirectoryWatcher->UnregisterDirectoryChangedCallback_Handle(FPaths::ProjectContentDir(), GameDirectoryWatcherHandle);
				GameDirectoryWatcherHandle.Reset();
			}
		}
	}
}

bool CBinaryDataRegistry::InitCheckFileDiff()
{
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();	
	CDirectoryStatVisitor cVisitor;
	bool bCheckFileValid = false;

	FString strSearchDir = FPaths::ProjectContentDir() / TEXT("GameContents/Data");
	//LOGDLE(TEXT("Search Directory : '%s'"), *strSearchDir);
	
	if(platformFile.IterateDirectoryStatRecursively(*strSearchDir, cVisitor))
	{
		if(!bCheckFileValid)
			bCheckFileValid = true;
	}

	strSearchDir = FPaths::ProjectContentDir() / TEXT("GameContents/Localization");
	//LOGDLE(TEXT("Search Directory : '%s'"), *strSearchDir);
	
	if(platformFile.IterateDirectoryStatRecursively(*strSearchDir, cVisitor))
	{
		if(!bCheckFileValid)
			bCheckFileValid = true;
	}

	return bCheckFileValid && m_instance->CheckFilesDiff();
}

void CBinaryDataRegistry::MakeCheckFiles(FString strFileName, const FFileStatData& StatData, int nExtentionIndex)
{
	//binary file
	if(nExtentionIndex == 0)
	{
		//convert relative path to absolute path
		strFileName = FPaths::ConvertRelativePathToFull(strFileName);

		// LOGDLE(TEXT("Check Bytes File '%s'"), *strFileName);
		
		m_mapTempBinaryModifyData.Add(strFileName, StatData.ModificationTime.GetTicks());
	}
	//uasset file
	else
	{
		FString strBaseFileName = FPaths::GetBaseFilename(strFileName);
		if(!strFileName.Contains(DataLoadLibInfo::BYTE_SUBFIX)
			&& !strFileName.Contains(DataLoadLibInfo::LZ_PREFIX))
			return;
		
		strFileName.RemoveFromEnd(FPaths::GetExtension(strFileName), ESearchCase::CaseSensitive);
		strFileName += strBaseFileName;
		strFileName.RemoveFromStart(FPaths::ProjectContentDir());
		strFileName = "/Game/" + strFileName;

		// LOGDLE(TEXT("Check UAsset File '%s'"), *strFileName)
		
		UBinaryDataSource* pData = LoadObject<UBinaryDataSource>(nullptr, *strFileName);
		if(pData)
		{
			SBinaryInfo sInfo;
			sInfo.lModifyTick = pData->ModifyTick;
			sInfo.strBinaryAssetName = pData->GetPathName();
			
			m_mapTempUaastBinaryData.Add(pData->FilePath, sInfo);
			// pData->ConditionalBeginDestroy();
		}
	}
}

bool CBinaryDataRegistry::CheckFilesDiff()
{
	for(auto& tuple : m_mapTempUaastBinaryData)
	{
		//Binary, Uaaset이 모두 존재할 때
		if(m_mapTempBinaryModifyData.Contains(tuple.Key))
		{
			//Modify Time Checck			
			if(m_mapTempBinaryModifyData[tuple.Key] == tuple.Value.lModifyTick)
			{
				//Remove temp
				m_mapTempBinaryModifyData.Remove(tuple.Key);
			}
		}
	}

	m_mapTempUaastBinaryData.Reset();

	// 남은 정보는 모두 변경되거나 추가된 것들.
	for(auto& tuple : m_mapTempBinaryModifyData)
	{
		//Add to registry
		m_arrRegistyFileName.Add(tuple.Key);
	}

	m_mapTempBinaryModifyData.Reset();

	for(FString& strFileName : m_arrRegistyFileName)
	{
		LOGDLE_WARNING(TEXT("Diff file Name : %s"), *strFileName);
	}

	return m_arrRegistyFileName.Num() > 0;
}

void CBinaryDataRegistry::OnDirectoryChanged(const TArray<FFileChangeData>& inFileChanges)
{
	if(m_arrRegistyFileName.Num() > 0)
		return;

	m_arrRegistyFileName.Reset();
	m_arrRegistryFilesAndDest.Reset();

	for(auto& data : inFileChanges)
	{
		if(data.Action == FFileChangeData::FCA_Added || data.Action == FFileChangeData::FCA_Modified)
		{
			if(m_pFactory->FactoryCanImport(data.Filename))
			{
				const FString& strToCompare = data.Filename;
				bool bDuplicated = false;
				for(auto& strFileName : m_arrRegistyFileName)
				{
					if(strToCompare == strFileName)
					{
						bDuplicated = true;
						break;
					}
				}

				if(!bDuplicated)
				{
					m_arrRegistyFileName.Add(data.Filename);
				}
			}
		}
	}

	RegistryAsset();
}

void CBinaryDataRegistry::MakeTick()
{
	m_TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &CBinaryDataRegistry::OnTick), 0);
}

bool CBinaryDataRegistry::OnTick(float fDeltaTime)
{
	// 다른 에셋들 로딩이 끝날 때 까지 대기 후 에셋 등록 및 수정.
	if(!IAssetRegistry::Get()->IsLoadingAssets())
	{
		RegistryAsset();
		RemoveTick();
	}
	
	return true;
}

void CBinaryDataRegistry::RemoveTick()
{
	FTicker::GetCoreTicker().RemoveTicker(m_TickDelegateHandle);
}

void CBinaryDataRegistry::RegistryAsset()
{
	if(m_arrRegistyFileName.Num() == 0)
		return;

	FText txtDialog = FText::FromString(TEXT("DataTool로 인한 파일 변경이 있습니다. 확인을 누르면 변경 내용이 적용됩니다."));
	FText txtTitle = FText::FromString("Binary Data Asset Import");

	FMessageDialog::Open(EAppMsgType::Ok, txtDialog, &txtTitle);

	FString strFilePathFromContents;

	for (auto& strFileName : m_arrRegistyFileName)
	{
		if(!GetContentsPathFromAbsolutePath(strFileName, strFilePathFromContents))
			continue;

		LOGDLE(TEXT("Import BinaryDataAsset from '%s'\n path '%s' ContentsPath '%s'"),
			*strFileName, *FPaths::GetPath(strFileName), *strFilePathFromContents);

		m_arrRegistryFilesAndDest.Add(TPair<FString, FString> { strFileName, strFilePathFromContents });		 
	}
	
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	TArray<UObject*> ImportedAssets = AssetToolsModule.Get().ImportAssets(m_arrRegistyFileName, TEXT("/Game"),
		m_pFactory, true, &m_arrRegistryFilesAndDest);

	m_arrRegistyFileName.Reset();
	m_arrRegistryFilesAndDest.Reset();
}

bool CBinaryDataRegistry::GetContentsPathFromAbsolutePath(const FString& strAbsolutePath, FString& strOutString)
{
	if(FPackageName::TryConvertFilenameToLongPackageName(strAbsolutePath, strOutString))
	{
		strOutString.RemoveFromEnd(FPaths::GetBaseFilename(strAbsolutePath), ESearchCase::CaseSensitive);
		return true;
	}
	
	return false;
}
