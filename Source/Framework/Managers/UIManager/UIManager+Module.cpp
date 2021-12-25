#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Managers/Managers.h"
#include "UIBase/UIModuleBase.h"

template <typename T>
T* UIManager::GetUIModule(const bool& bCreate)
{
	const auto& uclass = T::StaticClass();

	if(m_mapModulelInst.Contains(uclass))
		return Cast<T>(m_mapModulelInst[uclass]);

	if(!bCreate)
		return nullptr;

	FString strModuleName = T::StaticClass()->GetName();

	FString strRefPath = FString::Printf(TEXT("WidgetBlueprint'/Game/GameContents/UI/UIModules/%s/%s.%s_C'"), *strModuleName, *strModuleName, *strModuleName);
	auto bpClass = Managers::Asset->LoadBP<T>(strRefPath);
	if(bpClass == nullptr)
	{
		LOG_ERROR(TEXT("Failed :: Not Found BP :: %s"), *strRefPath);
		return nullptr;
	}

	auto pModule = Cast<T>(CreateWidget(Managers::GameInst, bpClass, *strModuleName));
	if(pModule == nullptr)
		return nullptr;

	m_mapModulelInst.Add(uclass, pModule);
	m_mapModuleRefCount.Add(uclass, 0);
	Managers::GameInst->AddWidget(pModule);

	pModule->OnUIModuleCreate(uclass);

	return pModule;
}

void UIManager::AddUIModuleRefCount(UClass* pModuleUClass)
{
	if(pModuleUClass == nullptr || !m_mapModuleRefCount.Contains(pModuleUClass))
		return;
	
	++m_mapModuleRefCount[pModuleUClass];
}

void UIManager::SubUIModuleRefCount(UClass* pModuleUClass)
{
	if(pModuleUClass == nullptr || !m_mapModuleRefCount.Contains(pModuleUClass))
		return;

	auto refCount = --m_mapModuleRefCount[pModuleUClass];

	// 레퍼런스 카운트가 0이면 삭제한다.
	if (refCount == 0)
	{
		Managers::GameInst->RemoveWidget(m_mapModulelInst[pModuleUClass]);
		m_mapModuleRefCount.Remove(pModuleUClass);
		m_mapModulelInst.Remove(pModuleUClass);
	}
}