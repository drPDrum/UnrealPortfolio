#include "DataLoadEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Type/BinaryDataTypeActions.h"

void FDataLoadEditor::StartupModule()
{
	// register custom types
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		// add custom category
		EAssetTypeCategories::Type ExampleCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("BinaryFile")), FText::FromString("Binary file from excel"));
		// register our custom asset with example category
		TSharedPtr<IAssetTypeActions> Action = MakeShareable(new FBinaryDataTypeActions(ExampleCategory));
		AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());		
		// saved it here for unregister later
		CreatedAssetTypeActions.Add(Action);
	}
}

void FDataLoadEditor::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
}

//IMPLEMENT_MODULE(FFrameworkEditor, FrameworkEditor);
IMPLEMENT_GAME_MODULE(FDataLoadEditor, DataLoadEditor);