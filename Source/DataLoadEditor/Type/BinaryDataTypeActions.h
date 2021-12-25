#pragma once
#include "AssetTypeActions_Base.h"

class UBinaryDataSource;

class FBinaryDataTypeActions : public FAssetTypeActions_Base
{
private:
	EAssetTypeCategories::Type MyAssetCategory;

public:
	FBinaryDataTypeActions(EAssetTypeCategories::Type inAssetCategory);

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;	
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	// End of IAssetTypeActions interface

	void ExecuteReimport(TArray<TWeakObjectPtr<UBinaryDataSource>> Objects);
};
