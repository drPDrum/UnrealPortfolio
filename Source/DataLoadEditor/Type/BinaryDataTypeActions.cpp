#include "BinaryDataTypeActions.h"

#include "EditorReimportHandler.h"
#include "DataLoadLib/BinaryDataSource.h"

FBinaryDataTypeActions::FBinaryDataTypeActions(EAssetTypeCategories::Type inAssetCategory)
	: MyAssetCategory(inAssetCategory) { }

FText FBinaryDataTypeActions::GetName() const
{
	return FText::FromString("Binary Data");
}

FColor FBinaryDataTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FBinaryDataTypeActions::GetSupportedClass() const
{
	return UBinaryDataSource::StaticClass();
}

uint32 FBinaryDataTypeActions::GetCategories()
{
	return MyAssetCategory;
}

bool FBinaryDataTypeActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FBinaryDataTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	auto dataImports = GetTypedWeakObjectPtrs<UBinaryDataSource>(InObjects);

	MenuBuilder.AddMenuEntry(
		FText::FromString("Reimport Binary Data"),
		FText::FromString("Reimports Binary Data"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateSP(this,
			&FBinaryDataTypeActions::ExecuteReimport, dataImports), FCanExecuteAction())
	);
}

void FBinaryDataTypeActions::ExecuteReimport(TArray<TWeakObjectPtr<UBinaryDataSource>> Objects)
{
	for(auto objIt = Objects.CreateConstIterator() ; objIt ; ++objIt)
	{
		auto obj = (*objIt).Get();
		if(obj)
			FReimportManager::Instance()->Reimport(obj, true);
	}
}
