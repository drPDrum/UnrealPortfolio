#pragma once
#include "EngineMinimal.h"
#include "Engine/ObjectLibrary.h"

template<class TAssetType = UObject>
void LoadUAssetAtPath(FString strPath, TArray<TAssetType*>& arrResult)
{
	{
		arrResult.Empty();
		// ObjLibrary로 해당 경로의 모든 파일 찾기
		auto objLibrary = UObjectLibrary::CreateLibrary(TAssetType::StaticClass(), true, true);
		objLibrary->LoadAssetDataFromPath(strPath);

		TArray<FAssetData> arrDatas;
		objLibrary->GetAssetDataList(arrDatas);

		for (int i =0 ; i < arrDatas.Num(); ++i)
		{
			auto assetData = arrDatas[i];
			auto refPath = FString::Printf(TEXT("%s'%s'"), *TAssetType::StaticClass()->GetName(), *assetData.ObjectPath.GetPlainNameString());
			TAssetType* uasset = LoadObject<TAssetType> (nullptr, *refPath);
			if (!uasset)
				continue;

			arrResult.Add(uasset);
		}
	}
}

template<typename UENUMTYPE>
FString UEnumToString(UENUMTYPE enumValue)
{
	FString text = typeid(UENUMTYPE).name();
	text.ReplaceInline(TEXT("enum "), TEXT(""));
	
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *text, true);
	if (!enumPtr)
		return FString("Invalid");

	return enumPtr->GetNameStringByValue(static_cast<int64>(enumValue));
}