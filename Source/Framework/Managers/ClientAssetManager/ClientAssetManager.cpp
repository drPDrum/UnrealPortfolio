#include "ClientAssetManager.h"
#include "Engine/ObjectLibrary.h"

ClientAssetManager::ClientAssetManager()
{
}

ClientAssetManager::~ClientAssetManager()
{
}

bool ClientAssetManager::Init()
{
	return IManagers::Init();
}

template <class TAssetType>
void ClientAssetManager::LoadUAssetAtPath(FString strPath, TArray<TAssetType*>& arrResult)
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

template <typename T>
T* ClientAssetManager::LoadObj(const FString& refPath)
{
	return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *refPath));
}

template <typename T>
UClass* ClientAssetManager::LoadBP(const FString& refPath)
{
	return StaticLoadClass(T::StaticClass(), nullptr, *refPath);
}