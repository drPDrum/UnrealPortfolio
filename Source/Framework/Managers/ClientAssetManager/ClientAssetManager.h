#pragma once
#include "Engine/ObjectLibrary.h"
#include "Framework/Managers/IManager.h"

class FRAMEWORK_API ClientAssetManager : IManagers
{
public:
	ClientAssetManager();
	virtual ~ClientAssetManager() override;

	virtual bool Init() override;

	template<class TAssetType = UObject>
	void LoadUAssetAtPath(FString strPath, TArray<TAssetType*>& arrResult);

	template<typename T = UClass>
	T* LoadObj(const FString& refPath);
	
	template<typename T = UClass>
	UClass* LoadBP(const FString& refPath);
};


