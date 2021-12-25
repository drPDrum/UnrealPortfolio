#pragma once
#include "BinaryDataFactory.h"
#include "EditorReimportHandler.h"
#include "ReimportBinaryDataFactory.generated.h"

UCLASS()
class DATALOADEDITOR_API UReimportBinaryDataFactory : public UBinaryDataFactory, public FReimportHandler
{
	GENERATED_BODY()

	// Begin FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	// End FReimportHandler interface
};
