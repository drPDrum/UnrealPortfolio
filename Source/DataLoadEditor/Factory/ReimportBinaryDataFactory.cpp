#include "ReimportBinaryDataFactory.h"
#include "DataLoadEditor/DataLoadEditorInfo.h"
#include "DataLoadLib/BinaryDataSource.h"
#include "EditorFramework/AssetImportData.h"

bool UReimportBinaryDataFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UBinaryDataSource* uData = Cast<UBinaryDataSource>(Obj);

	if(uData)
	{
		OutFilenames.Add(UAssetImportData::ResolveImportFilename(uData->FilePath, uData->GetOutermost()));
		return true;
	}

	return false;
}
void UReimportBinaryDataFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UBinaryDataSource* uData = Cast<UBinaryDataSource>(Obj);

	if(uData && ensure(NewReimportPaths.Num() == 1))
	{
		uData->FilePath = UAssetImportData::SanitizeImportFilename(NewReimportPaths[0], uData->GetOutermost());

		FFileStatData sFileStat = IFileManager::Get().GetStatData(*uData->FilePath);
		uData->ModifyTick = sFileStat.ModificationTime.GetTicks();
	}
}

EReimportResult::Type UReimportBinaryDataFactory::Reimport(UObject* Obj)
{
	UBinaryDataSource* uData = Cast<UBinaryDataSource>(Obj);

	if(!uData)
		return EReimportResult::Failed;

	const FString strFilename = UAssetImportData::ResolveImportFilename(uData->FilePath,
		uData->GetOutermost());

	if(!FPaths::GetExtension(strFilename).Equals(DataLoadLibInfo::BYTE_EXTS))
		return EReimportResult::Failed;

	CurrentFilename = strFilename;

	if(uData->SetDataFromFile(CurrentFilename))
	{
		uData->Modify();
		uData->FilePath = CurrentFilename;
		FFileStatData sFileStat = IFileManager::Get().GetStatData(*uData->FilePath);
		uData->ModifyTick = sFileStat.ModificationTime.GetTicks();
		
		return EReimportResult::Succeeded;		
	}
	else
		return EReimportResult::Failed;
}
