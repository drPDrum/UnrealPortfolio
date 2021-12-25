#include "FrameworkGameInst.h"

#include "Blueprint/UserWidget.h"
#include "Framework/Managers/Managers.h"

void UFrameworkGameInst::Init()
{
	Super::Init();
}

void UFrameworkGameInst::BeginDestroy()
{
	Managers::Release();
	
	Super::BeginDestroy();
}

void UFrameworkGameInst::AddWidget(UUserWidget* pUserWidget)
{
	if(pUserWidget == nullptr)
		return;
	
	if(m_setWidgetInst.Contains(pUserWidget))
		return;

	m_setWidgetInst.Add(pUserWidget);
}

void UFrameworkGameInst::RemoveWidget(UUserWidget* pUserWidget)
{
	if(pUserWidget == nullptr)
		return;
	
	if(m_setWidgetInst.Contains(pUserWidget))
	{
		m_setWidgetInst.Remove(pUserWidget);
		pUserWidget->ConditionalBeginDestroy();
	}
}

void UFrameworkGameInst::ClearWidgets()
{
	TArray<UUserWidget*> delWidgets;

	for(auto& pWidget : m_setWidgetInst)
		delWidgets.Add(pWidget);

	m_setWidgetInst.Empty();
	
	for (auto& pWidget : delWidgets)
		pWidget->ConditionalBeginDestroy();
}

void UFrameworkGameInst::AddMat(UMaterialInterface* pMat)
{
	if (m_setMaterial.Contains(pMat))
		return;

	m_setMaterial.Add(pMat);
}

void UFrameworkGameInst::RemoveMat(UMaterialInterface* pMat)
{
	if (m_setMaterial.Contains(pMat))
	{
		m_setMaterial.Remove(pMat);
		pMat->ConditionalBeginDestroy();
	}
}

void UFrameworkGameInst::RemoveMat(const TArray<UMaterialInterface*>& mats)
{
	for (int i = 0 ; i < mats.Num(); ++i)
		RemoveMat(mats[i]);
}

void UFrameworkGameInst::RemoveMat(const TArray<UMaterialInstanceDynamic*>& mats)
{
	for (int i = 0 ; i < mats.Num(); ++i)
		RemoveMat(mats[i]);
}

void UFrameworkGameInst::ClearMat()
{
	TArray<UMaterialInterface*> delMats;
	{
		for(auto& pMat : m_setMaterial)
			delMats.Add(pMat);
	}
	
	m_setMaterial.Empty();
	
	{
		for (auto& pMat : delMats)
			pMat->ConditionalBeginDestroy();
	}
}

void UFrameworkGameInst::OnCompleteLoadLevel()
{
	if(Managers::IsInit())
		Managers::World->LoadCompleteLevel();
}
