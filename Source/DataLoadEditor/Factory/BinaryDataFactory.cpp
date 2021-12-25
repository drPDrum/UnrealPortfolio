#include "BinaryDataFactory.h"
#include "AssetImportTask.h"
#include "DataLoadEditor/DataLoadEditorInfo.h"
#include "DataLoadEditor/Registry/CBinaryDataRegistry.h"
#include "DataLoadLib/BinaryDataSource.h"

UBinaryDataFactory::UBinaryDataFactory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Formats.Add(TEXT("bytes;Binary Data File"));
	
	SupportedClass = UBinaryDataSource::StaticClass();

	bCreateNew = false;
	bEditAfterNew = false;
	bEditorImport = true;
	// bText = true;

	m_pRegistry = CBinaryDataRegistry::Instance(this);
}

bool UBinaryDataFactory::FactoryCanImport(const FString& Filename)
{
	return FPaths::GetExtension(Filename).Equals(DataLoadLibInfo::BYTE_EXTS);
}

UObject* UBinaryDataFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{	
	// This function performs shortcut to call FactoryCreateBinary without loading a file to array.
	FString FileExtension = FPaths::GetExtension(Filename);

	LOGDLE(TEXT("BinaryDataFactory Create :: file name '%s' Class '%s' Parent '%s' InName '%s'"),
		*Filename, *InClass->GetName(), *InParent->GetName(), *InName.ToString());
	
	if (!IFileManager::Get().FileExists(*Filename))
	{
		LOGDLE_ERROR(TEXT("Failed to load file '%s'"), *Filename);
		
		return nullptr;
	}

	UBinaryDataSource* pData = NewObject<UBinaryDataSource>(InParent, InClass, InName, Flags);
	pData->SetDataFromFile(Filename);
	
	return pData;
}