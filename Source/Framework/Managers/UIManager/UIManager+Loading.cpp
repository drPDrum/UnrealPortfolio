#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Managers/Managers.h"
#include "UIBase/LoadingBase.h"

ULoadingBase* UIManager::OpenLoading(const FString& strLoadingName)
{
	if(strLoadingName.IsEmpty())
		return nullptr;

	// LoadingPage Instance 생성
	FString strRefPath = FString::Printf(TEXT("WidgetBlueprint'/Game/GameContents/UI/LoadingPage/%s/%s.%s_C'"), *strLoadingName, *strLoadingName, *strLoadingName);
	auto bpClass = Managers::Asset->LoadBP<ULoadingBase>(strRefPath);
	if (!bpClass)
		return nullptr;

	auto pNewLoadingPage = Cast<ULoadingBase>(CreateWidget(Managers::GameInst, bpClass, *strLoadingName));
	if(pNewLoadingPage == nullptr)
		return nullptr;

	// 한번 쓰고 버리도록 구현되어 있다. 게임 상황에 맞춰서 변경하자
	// 로딩 페이지 띄우기
	pNewLoadingPage->AddToViewport(1000);
	Managers::GameInst->AddWidget(pNewLoadingPage);

	return pNewLoadingPage;
}

void UIManager::CloseLoading(ULoadingBase* pLoadingPage)
{
	if (pLoadingPage)
	{
		// 한번 쓰고 버리도록 구현되어 있다. 게임 상황에 맞춰서 변경하자
		// 로딩 페이지 지우기
		pLoadingPage->RemoveFromViewport();
		Managers::GameInst->RemoveWidget(pLoadingPage);
	}
}


